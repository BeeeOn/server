#include <Poco/Exception.h>

#include "di/InjectorTarget.h"
#include "di/AbstractInjectorTarget.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

AbstractInjectorTarget::~AbstractInjectorTarget()
{
	RefSetterMap::iterator it = m_refSetter.begin();

	for (; it != m_refSetter.end(); ++it)
		delete it->second;
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

	TextSetter func = it->second;
	(this->*func)(value);
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

	NumberSetter func = it->second;
	(this->*func)(value);
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
