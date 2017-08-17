#include <cstring>

#include <unicode/unistr.h>
#include <unicode/ures.h>

#include <Poco/Exception.h>

#include "l10n/IcuLocaleImpl.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

IcuLocaleImpl::IcuLocaleImpl():
	m_locale(icu::Locale::getRoot())
{
}

IcuLocaleImpl::IcuLocaleImpl(const string &language, const string &country):
	m_locale(language.c_str(), country.c_str())
{
}

IcuLocaleImpl::IcuLocaleImpl(const icu::Locale &loc):
	m_locale(loc)
{
}

string IcuLocaleImpl::language() const
{
	return m_locale.getLanguage();
}

string IcuLocaleImpl::country() const
{
	return m_locale.getCountry();
}

string IcuLocaleImpl::displayName() const
{
	icu::UnicodeString result;
	m_locale.getDisplayName(m_locale, result);

	string utf8;
	return result.toUTF8String(utf8);
}

string IcuLocaleImpl::toString() const
{
	return m_locale.getBaseName();
}

bool IcuLocaleImpl::lessThen(const IcuLocaleImpl &other) const
{
	if (m_locale == other.m_locale)
		return false;

	const int ret = strcmp(m_locale.getName(), other.m_locale.getName());
	return ret < 0;
}

bool IcuLocaleImpl::equals(const IcuLocaleImpl &other) const
{
	return m_locale == other.m_locale;
}

IcuLocaleImpl IcuLocaleImpl::parse(const std::string &input)
{
	return IcuLocaleImpl(icu::Locale::createFromName(input.c_str()));
}

void IcuLocaleImpl::available(const string &bundleName, list<IcuLocaleImpl> &locales)
{
	UErrorCode error = U_ZERO_ERROR;
	UEnumeration *e = ures_openAvailableLocales(
				bundleName.c_str(), &error);

	if (error == U_MISSING_RESOURCE_ERROR) {
		uenum_close(e);
		throw NotFoundException("missing resource res_index");
	}

	if (!U_SUCCESS(error)) {
		uenum_close(e);
		throw IllegalStateException(u_errorName(error));
	}

	const char *current = NULL;
	int32_t length = 0;

	while ((current = uenum_next(e, &length, &error)) != NULL) {
		if (!U_SUCCESS(error)) {
			uenum_close(e);
			throw IllegalStateException(u_errorName(error));
		}

		locales.push_back(IcuLocaleImpl::parse(string(current, length)));
	}

	uenum_close(e);
}
