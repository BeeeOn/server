#include <Poco/Exception.h>

#include "di/InjectorTarget.h"
#include "Debug.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

ManifestSingleton *ManifestSingleton::singleton = NULL;

static void __attribute__((destructor,used)) ManifestSingleton_destroy()
{
	ManifestSingleton::destroy();
}

void ManifestSingleton::reportInfo(Logger &logger)
{
	logger.debug("managing " + to_string(manifest().size()) + " classes in " + manifest().className());
	Manifest<InjectorTarget>::Iterator it = manifest().begin();

	for (; it != manifest().end(); ++it) {
		string msg("registered class ");
		logger.debug(msg + it->name(), __FILE__, __LINE__);
	}
}

AbstractInjectorTarget::~AbstractInjectorTarget()
{
	RefSetterMap::iterator it = m_refSetter.begin();

	for (; it != m_refSetter.end(); ++it)
		delete it->second;
}

void AbstractInjectorTarget::injectRef(
		const std::string &key, InjectorTarget *value)
{
	RefSetterMap::iterator it = m_refSetter.find(key);

	if (it == m_refSetter.end())
		throw NotFoundException("missing setter in " + key);

	it->second->call(this, value);
}

void AbstractInjectorTarget::injectText(
		const string &key, const string &value)
{
	TRACE_METHOD();

	TextSetterMap::iterator it = m_textSetter.find(key);

	if (it == m_textSetter.end())
		throw NotFoundException("missing text setter " + key);

	TextSetter func = it->second;
	(this->*func)(value);
}

void AbstractInjectorTarget::injectNumber(
		const string &key, int value)
{
	TRACE_METHOD();

	NumberSetterMap::iterator it = m_numberSetter.find(key);

	if (it == m_numberSetter.end())
		throw NotFoundException("missing number setter " + key);

	NumberSetter func = it->second;
	(this->*func)(value);
}
