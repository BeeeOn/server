#pragma once

#include <list>

#include <unicode/locid.h>

#include "l10n/LocaleImpl.h"

namespace BeeeOn {

class IcuLocaleImpl : public LocaleImpl {
public:
	/**
	 * Construct the root or default locale.
	 */
	IcuLocaleImpl();
	IcuLocaleImpl(const std::string &language, const std::string &country);
	IcuLocaleImpl(const icu::Locale &loc);

public:
	/**
	 * Returns language code of the locale.
	 */
	std::string language() const override;

	/**
	 * Returns country code of the locale.
	 */
	std::string country() const override;

	/**
	 * Name to display to user.
	 *
	 * It displays its name via its locale. Thus if the locale is
	 * en_US, you would get English (USA). If the locale is cs_CZ,
	 * the result would be Čeština (Česká republika).
	 */
	std::string displayName() const override;

	/**
	 * String representation of the locale.
	 * It is usually a code like en_US.
	 */
	std::string toString() const override;

	/**
	 * Sorting support.
	 */
	bool lessThen(const IcuLocaleImpl &other) const;

	bool operator <(const IcuLocaleImpl &other) const
	{
		return lessThen(other);
	}

	/**
	 * Equality support.
	 */
	bool equals(const IcuLocaleImpl &other) const;

	bool operator ==(const IcuLocaleImpl &other) const
	{
		return equals(other);
	}

	bool operator !=(const IcuLocaleImpl &other) const
	{
		return !equals(other);
	}

	/**
	 * Parse the given input and expect it to represent a locale.
	 */
	static IcuLocaleImpl parse(const std::string &input);

	/**
	 * List locales available for the given bundle name.
	 */
	static void available(const std::string &bundleName,
			std::list<IcuLocaleImpl> &locales);

	/**
	 * Access the implementation-specific locale representation.
	 */
	icu::Locale icu() const
	{
		return m_locale;
	}

private:
	icu::Locale m_locale;
};

}
