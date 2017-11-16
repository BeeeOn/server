#include <list>

#include <unicode/putil.h>

#include <Poco/Exception.h>
#include <Poco/Logger.h>

#include "di/Injectable.h"
#include "l10n/IcuLocaleImpl.h"
#include "l10n/IcuLocaleManager.h"

BEEEON_OBJECT_BEGIN(BeeeOn, IcuLocaleManager)
BEEEON_OBJECT_CASTABLE(LocaleManager)
BEEEON_OBJECT_TEXT("bundleName", &IcuLocaleManager::setBundleName)
BEEEON_OBJECT_TEXT("defaultLocale", &IcuLocaleManager::setDefaultLocale)
BEEEON_OBJECT_TEXT("resourceDir", &IcuLocaleManager::setResourceDir)
BEEEON_OBJECT_HOOK("done", &IcuLocaleManager::setup)
BEEEON_OBJECT_END(BeeeOn, IcuLocaleManager)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

IcuLocaleManager::IcuLocaleManager()
{
}

SharedPtr<IcuLocaleImpl> IcuLocaleManager::determine(const IcuLocaleImpl &locale) const
{
	auto it = m_availableLocales.find(locale.toString());
	if (it != m_availableLocales.end())
		return it->second;

	it = m_availableLocales.find(locale.language());
	if (it != m_availableLocales.end())
		return it->second;

	return NULL;
}

BeeeOn::Locale IcuLocaleManager::defaultLocale()
{
	if (m_cachedDefaultLocale.isNull())
		m_cachedDefaultLocale = new IcuLocaleImpl(IcuLocaleImpl::parse(m_defaultLocale));

	return Locale(m_cachedDefaultLocale);
}

BeeeOn::Locale IcuLocaleManager::parse(const string &input)
{
	IcuLocaleImpl locale = IcuLocaleImpl::parse(input);
	auto found = determine(locale);

	if (found.isNull()) {
		if (logger().debug()) {
			logger().warning("requested missing locale: " + locale.toString(),
				__FILE__, __LINE__);
		}

		return defaultLocale();
	}

	return Locale(found);
}

BeeeOn::Locale IcuLocaleManager::chooseBest(const vector<string> &input)
{
	for (const auto one : input) {
		IcuLocaleImpl locale = IcuLocaleImpl::parse(one);
		auto found = determine(locale);

		if (found.isNull())
			continue;

		return Locale(found);
	}

	return defaultLocale();
}

void IcuLocaleManager::setDefaultLocale(const string &name)
{
	m_defaultLocale = name;
}

void IcuLocaleManager::setBundleName(const string &name)
{
	m_bundleName = name;
}

void IcuLocaleManager::setResourceDir(const string &dir)
{
	u_setDataDirectory(dir.c_str());
}

void IcuLocaleManager::setup()
{
	UVersionInfo version;
	u_getVersion(version);

	logger().notice("using ICU "
		+ to_string((int) version[0])
		+ "."
		+ to_string((int) version[1])
		+ "."
		+ to_string((int) version[2])
		+ "."
		+ to_string((int) version[3]),
		__FILE__, __LINE__);

	logger().information("setup resource bundle: " + m_bundleName,
			__FILE__, __LINE__);

	logger().information("resource dir: " + string(u_getDataDirectory()),
			__FILE__, __LINE__);

	list<IcuLocaleImpl> available;
	IcuLocaleImpl::available(m_bundleName, available);

	for (const auto &loc : available) {
		logger().information("detected locale: " + loc.toString(),
			__FILE__, __LINE__);

		m_availableLocales.emplace(loc.toString(), new IcuLocaleImpl(loc));
	}

	const IcuLocaleImpl defaultLocale = IcuLocaleImpl::parse(m_defaultLocale);

	for (const auto &loc : available) {
		if (loc == defaultLocale) {
			installDefaultLocale(defaultLocale);
			return;
		}
	}

	throw IllegalStateException("default locale "
			+ defaultLocale.toString()
			+ " was not found");
}

void IcuLocaleManager::installDefaultLocale(const IcuLocaleImpl &locale)
{
	UErrorCode error = U_ZERO_ERROR;
	icu::Locale::setDefault(locale.icu(), error);
	if (!U_SUCCESS(error)) {
		throw IllegalStateException(
			string("setting default locale: ")
			+ u_errorName(error));
	}

	m_cachedDefaultLocale = new IcuLocaleImpl(locale);

	logger().information("default locale: " + locale.toString(),
			__FILE__, __LINE__);
}
