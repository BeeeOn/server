#include <set>

#include "di/DIWrapper.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

DIWDuplicateException::DIWDuplicateException(
		const string &key,
		const type_info &method):
	m_text(string("duplicate property ")
		+ key + " with method " + method.name())
{
}

const char *DIWDuplicateException::what() const noexcept
{
	return m_text.c_str();
}

DIWCastException::DIWCastException(
		const type_info &from,
		const type_info &to):
	m_text(string("could not cast ")
		+ from.name() + " to " + to.name())
{
}

const char *DIWCastException::what() const noexcept
{
	return m_text.c_str();
}

DIWMethodHelper::~DIWMethodHelper()
{
}

DIWNumberSetter::~DIWNumberSetter()
{
}

DIWTextSetter::~DIWTextSetter()
{
}

DIWRefSetter::~DIWRefSetter()
{
}

DIWHook::~DIWHook()
{
}

DIWrapper::~DIWrapper()
{
}

class DIWCastRegistry {
public:
	~DIWCastRegistry()
	{
		for (auto cast : m_cast)
			delete cast;
	}

	void add(DIWCast *cast)
	{
		m_cast.insert(cast);
	}

	set<DIWCast *>::iterator begin()
	{
		return m_cast.begin();
	}

	set<DIWCast *>::iterator end()
	{
		return m_cast.end();
	}

private:
	set<DIWCast *> m_cast;
};

static DIWCastRegistry g_registry;

DIWCast::~DIWCast()
{
}

void DIWCast::add(DIWCast *cast)
{
	g_registry.add(cast);
}

DIWCast *DIWCast::find(const type_info &info, DIWrapper &wrapper)
{
	for (auto cast : g_registry) {
		if (cast->wouldCast(info) && cast->isSame(wrapper))
			return cast;
	}

	return NULL;
}

ManifestSingleton *ManifestSingleton::singleton = NULL;

static void __attribute__((destructor,used)) ManifestSingleton_destroy()
{
	ManifestSingleton::destroy();
}

void ManifestSingleton::reportInfo(Logger &logger)
{
	logger.debug("managing " + to_string(manifest().size()) + " classes in " + manifest().className());
	Manifest<DIWrapper>::Iterator it = manifest().begin();

	for (; it != manifest().end(); ++it) {
		string msg("registered class ");
		logger.debug(msg + it->name(), __FILE__, __LINE__);
	}
}
