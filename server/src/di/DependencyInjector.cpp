#include <Poco/Exception.h>

#include "di/DependencyInjector.h"
#include "Debug.h"

using namespace std;
using namespace Poco;
using namespace Poco::Util;
using namespace BeeeOn;

namespace BeeeOn {

class InstanceInfo {
public:
	InstanceInfo(const string &name):
		m_name(name)
	{
	}

	const string &name() const
	{
		return m_name;
	}

	void validate(AutoPtr<AbstractConfiguration> conf) const
	{
		const string base("instance[@name='" + m_name + "']");

		if (!conf->has(base)) {
			throw NotFoundException(
				"missing instance of name " + m_name);
		}

		if (!conf->has(base + "[@class]")) {
			throw NotFoundException(
				"attribute 'class' not specified for instance "
				+ m_name);
		}
	}

	const string resolveAlias(AutoPtr<AbstractConfiguration> conf) const
	{
		const string ref("alias[@name='" + m_name + "'][@ref]");
		const string &value = conf->getString(ref, "");

		if (value.compare(m_name) == 0)
			throw IllegalStateException("alias " + m_name
					+ " refers to itself");

		return value;
	}

	const string resolveClass(AutoPtr<AbstractConfiguration> conf) const
	{
		string entry("instance[@name='" + m_name + "'][@class]");
		return conf->getString(entry);
	}

	void resolveKeys(AutoPtr<AbstractConfiguration> conf,
			AbstractConfiguration::Keys &keys) const
	{
		const string base("instance[@name='" + m_name + "']");

		AbstractConfiguration::Keys tmp;
		conf->keys(base, tmp);

		AbstractConfiguration::Keys::const_iterator it;
		for (it = tmp.begin(); it != tmp.end(); ++it) {
			const string &key = *it;
			if (key.find("set") != 0 && key.find("add") != 0)
				continue;

			keys.push_back(base + "." + key);
		}
	}

private:
	string m_name;
};

}

DependencyInjector::~DependencyInjector()
{
	InjectorSet::iterator it;
	for (it = m_set.begin(); it != m_set.end(); ++it) {
		InjectorTarget *t = it->second;
		delete t; // order is not preserved here
	}
}

void DependencyInjector::createEarly()
{
	AbstractConfiguration::Keys tmp;
	m_conf->keys(tmp);

	AbstractConfiguration::Keys::const_iterator it;
	for (it = tmp.begin(); it != tmp.end(); ++it) {
		const string &key = *it;

		if (key.find("instance[") == string::npos)
			continue;

		const string &init = m_conf->getString(
				key + "[@init]", "lazy");
		if (init.compare("early"))
			continue;

		const string name = m_conf->getString(key + "[@name]");
		(void) createNoAlias(name);
	}
}

InjectorTarget *DependencyInjector::create(const string &name, bool disown)
{
	TRACE_METHOD();

	InjectorTarget *existing = find(name);
	if (existing != NULL) {
		m_logger.debug("instance " + name + " reused",
				__FILE__, __LINE__);
		return existing;
	}

	InstanceInfo info(name);
	const string &ref = info.resolveAlias(m_conf);

	if (ref.empty())
		return createNoAlias(info, disown);

	existing = find(ref);
	if (existing != NULL) {
		m_logger.debug("instance " + name
				+ " reused as alias to " + ref,
				__FILE__, __LINE__);
		return existing;
	}

	InstanceInfo aliasInfo(ref);
	return createNoAlias(aliasInfo, disown);
}

InjectorTarget *DependencyInjector::find(const string &name)
{
	InjectorSet::const_iterator it = m_set.find(name);
	if (it != m_set.end())
		return it->second;

	return NULL;
}

InjectorTarget *DependencyInjector::createNoAlias(
		const InstanceInfo &info, bool disown)
{
	InjectorTarget *t = createNew(info);
	if (t == NULL)
		throw Poco::NullPointerException("failed to create target "
				+ info.name());

	if (!disown) {
		m_set.insert(make_pair(info.name(), t));
	}

	return injectDependencies(info, t);
}

InjectorTarget *DependencyInjector::createNew(const InstanceInfo &info)
{
	info.validate(m_conf);
	const string cls = info.resolveClass(m_conf);

	m_logger.debug("creating " + info.name() + " as " + cls);

	Manifest<InjectorTarget>::Iterator it =
		ManifestSingleton::manifest().find(cls);

	if (it == ManifestSingleton::manifest().end())
		throw NotFoundException("missing class " + cls);

	return it->create();
}

bool DependencyInjector::tryInjectRef(
		const InstanceInfo &info,
		InjectorTarget *target,
		const string &key,
		const string &name)
{
	if (m_conf->has(key + "[@ref]")) {
		const string value = m_conf->getString(key + "[@ref]");

		m_logger.debug("injecting " + value + " as " + name
				+ " into " + info.name());

		InjectorTarget *ref = create(value);
		if (ref == NULL) {
			throw NullPointerException(
					"failed to create ref " + value);
		}

		target->injectRef(name, ref);
		return true;
	}

	return false;
}

bool DependencyInjector::tryInjectNumber(
		const InstanceInfo &info,
		InjectorTarget *target,
		const string &key,
		const string &name)
{
	if (m_conf->has(key + "[@number]")) {
		const int value = m_conf->getInt(key + "[@number]");

		m_logger.debug("injecting number " + to_string(value)
				+ " as " + name + " into " + info.name());

		target->injectNumber(name, value);
		return true;
	}

	return false;
}

bool DependencyInjector::tryInjectText(
		const InstanceInfo &info,
		InjectorTarget *target,
		const string &key,
		const string &name)
{
	if (m_conf->has(key + "[@text]")) {
		const string value = m_conf->getString(key + "[@text]");

		m_logger.debug("injecting " + value + " as " + name
				+ " into " + info.name());

		target->injectText(name, value);
		return true;
	}

	return false;
}

void DependencyInjector::injectValue(
		const InstanceInfo &info,
		InjectorTarget *target,
		const string &key)
{
	if (!m_conf->has(key + "[@name]"))
		return;

	const string name = m_conf->getString(key + "[@name]");

	if (tryInjectRef(info, target, key, name))
		return;

	if (tryInjectNumber(info, target, key, name))
		return;

	if (tryInjectText(info, target, key, name))
		return;

	m_logger.error("malformed configuration entry "
			+ key + " for " + info.name(),
			__FILE__, __LINE__);
}

InjectorTarget *DependencyInjector::injectDependencies(
		const InstanceInfo &info,
		InjectorTarget *target)
{
	AbstractConfiguration::Keys keys;
	info.resolveKeys(m_conf, keys);

	AbstractConfiguration::Keys::const_iterator it;
	for (it = keys.begin(); it != keys.end(); ++it) {
		const string &key = *it;

		m_logger.trace("visiting key " + key);
		injectValue(info, target, key);
		m_logger.trace("next key after " + key);
	}

	m_logger.notice("successfully created " + info.name(),
			__FILE__, __LINE__);

	target->injectionDone();
	return target;
}
