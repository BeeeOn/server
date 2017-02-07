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

	bool testConditions(AutoPtr<AbstractConfiguration> conf,
			const string &key) const
	{
		const string if_yes(key + "[@if-yes]");

		/* no such condition */
		if (!conf->has(if_yes))
			return true;

		const string &value = conf->getString(if_yes);
		return value == "y" || value == "yes" || value == "true";
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

			if (!testConditions(conf, base + "." + key))
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
	m_set.clear();

	while (!m_free.empty())
		m_free.pop_back();
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

SharedPtr<InjectorTarget> DependencyInjector::create(const string &name, bool disown)
{
	TRACE_METHOD();

	SharedPtr<InjectorTarget> existing = find(name);
	if (!existing.isNull()) {
		logger().debug("instance " + name + " reused",
				__FILE__, __LINE__);
		return existing;
	}

	InstanceInfo info(name);
	const string &ref = info.resolveAlias(m_conf);

	if (ref.empty())
		return createNoAlias(info, disown);

	existing = find(ref);
	if (!existing.isNull()) {
		logger().debug("instance " + name
				+ " reused as alias to " + ref,
				__FILE__, __LINE__);
		return existing;
	}

	InstanceInfo aliasInfo(ref);
	return createNoAlias(aliasInfo, disown);
}

SharedPtr<InjectorTarget> DependencyInjector::find(const string &name)
{
	InjectorSet::const_iterator it = m_set.find(name);
	if (it != m_set.end())
		return it->second;

	return NULL;
}

SharedPtr<InjectorTarget> DependencyInjector::createNoAlias(
		const InstanceInfo &info, bool disown)
{
	SharedPtr<InjectorTarget> t = createNew(info);
	if (t.isNull())
		throw Poco::NullPointerException("failed to create target "
				+ info.name());

	if (!disown) {
		m_set.insert(make_pair(info.name(), t));
		m_free.push_back(t);
	}

	return injectDependencies(info, t);
}

InjectorTarget *DependencyInjector::createNew(const InstanceInfo &info)
{
	info.validate(m_conf);
	const string cls = info.resolveClass(m_conf);

	logger().debug("creating " + info.name() + " as " + cls);

	Manifest<InjectorTarget>::Iterator it =
		ManifestSingleton::manifest().find(cls);

	if (it == ManifestSingleton::manifest().end())
		throw NotFoundException("missing class " + cls);

	return it->create();
}

bool DependencyInjector::tryInjectRef(
		const InstanceInfo &info,
		SharedPtr<InjectorTarget> target,
		const string &key,
		const string &name)
{
	if (m_conf->has(key + "[@ref]")) {
		const string value = m_conf->getString(key + "[@ref]");

		logger().debug("injecting " + value + " as " + name
				+ " into " + info.name());

		SharedPtr<InjectorTarget> ref;

		try {
			ref = create(value);
		} catch (const Exception &e) {
			logger().error("failed to create ref " + value,
					__FILE__, __LINE__);
			e.rethrow();
		}

		if (ref.isNull()) {
			throw NullPointerException(
					"failed to create ref " + value);
		}

		target->injectRef(name, ref.get());
		return true;
	}

	return false;
}

bool DependencyInjector::tryInjectNumber(
		const InstanceInfo &info,
		SharedPtr<InjectorTarget> target,
		const string &key,
		const string &name)
{
	if (m_conf->has(key + "[@number]")) {
		const int value = m_conf->getInt(key + "[@number]");

		logger().debug("injecting number " + to_string(value)
				+ " as " + name + " into " + info.name());

		target->injectNumber(name, value);
		return true;
	}

	return false;
}

bool DependencyInjector::tryInjectText(
		const InstanceInfo &info,
		SharedPtr<InjectorTarget> target,
		const string &key,
		const string &name)
{
	if (m_conf->has(key + "[@text]")) {
		const string value = m_conf->getString(key + "[@text]");

		logger().debug("injecting " + value + " as " + name
				+ " into " + info.name());

		target->injectText(name, value);
		return true;
	}

	return false;
}

void DependencyInjector::injectValue(
		const InstanceInfo &info,
		SharedPtr<InjectorTarget> target,
		const string &key,
		const string &name)
{
	if (tryInjectRef(info, target, key, name))
		return;

	if (tryInjectNumber(info, target, key, name))
		return;

	if (tryInjectText(info, target, key, name))
		return;

	logger().error("malformed configuration entry "
			+ key + " for " + info.name(),
			__FILE__, __LINE__);
}

SharedPtr<InjectorTarget> DependencyInjector::injectDependencies(
		const InstanceInfo &info,
		SharedPtr<InjectorTarget> target)
{
	AbstractConfiguration::Keys keys;
	info.resolveKeys(m_conf, keys);

	AbstractConfiguration::Keys::const_iterator it;
	for (it = keys.begin(); it != keys.end(); ++it) {
		const string &key = *it;

		logger().trace("visiting key " + key);

		if (!m_conf->has(key + "[@name]")) {
			logger().warning("missing @name for " + key,
					__FILE__, __LINE__);
			continue;
		}

		const string name = m_conf->getString(key + "[@name]");

		try {
			injectValue(info, target, key, name);
		} catch (const Poco::Exception &e) {
			logger().error("failed inject " + name + " for "
					+ info.name(), __FILE__, __LINE__);
			e.rethrow();
		}

		logger().trace("next key after " + key);
	}

	logger().notice("successfully created " + info.name(),
			__FILE__, __LINE__);

	try {
		target->injectionDone();
	} catch (const Exception &e) {
		logger().error("injectionDone() failed for " + info.name(),
				__FILE__, __LINE__);
		e.rethrow();
	}

	return target;
}
