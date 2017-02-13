#include <Poco/Exception.h>

#include "di/InjectorTarget.h"
#include "di/AbstractInjectorTarget.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

AbstractInjectorTarget::~AbstractInjectorTarget()
{
	auto refIt = m_refSetter.begin();

	for (; refIt != m_refSetter.end(); ++refIt)
		delete refIt->second;

	auto numberIt = m_numberSetter.begin();

	for (; numberIt != m_numberSetter.end(); ++numberIt)
		delete numberIt->second;

	auto textIt = m_textSetter.begin();

	for (; textIt != m_textSetter.end(); ++textIt)
		delete textIt->second;

	auto hookIt = m_hookMap.begin();

	for (; hookIt != m_hookMap.end(); ++hookIt)
		delete hookIt->second;
}

void AbstractInjectorTarget::injectRef(
		const std::string &key, SharedPtr<InjectorTarget> value)
{
	RefSetterMap::iterator it = m_refSetter.find(key);

	if (it == m_refSetter.end()) {
		if (injectRefFallback(key, value))
			return;

		throw NotFoundException("missing setter for " + key);
	}

	it->second->call(this, value);
}

void AbstractInjectorTarget::injectText(
		const string &key, const string &value)
{
	TextSetterMap::iterator it = m_textSetter.find(key);

	if (it == m_textSetter.end()) {
		if (injectTextFallback(key, value))
			return;

		throw NotFoundException("missing text setter for " + key);
	}

	it->second->call(this, value);
}

void AbstractInjectorTarget::injectNumber(
		const string &key, int value)
{
	NumberSetterMap::iterator it = m_numberSetter.find(key);

	if (it == m_numberSetter.end()) {
		if (injectNumberFallback(key, value))
			return;

		throw NotFoundException("missing number setter for " + key);
	}

	it->second->call(this, value);
}

bool AbstractInjectorTarget::injectRefFallback(
			const std::string &key,
			SharedPtr<InjectorTarget> value)
{
	return false;
}

bool AbstractInjectorTarget::injectTextFallback(
			const std::string &key,
			const std::string &value)
{
	return false;
}

bool AbstractInjectorTarget::injectNumberFallback(
			const std::string &key,
			int value)
{
	return false;
}

void AbstractInjectorTarget::callHook(const std::string &key, bool required)
{
	HookMap::iterator it = m_hookMap.find(key);

	if (it == m_hookMap.end()) {
		if (!required)
			return;

		throw NotFoundException("no such hook defined " + key);
	}

	it->second->call(this);
}
