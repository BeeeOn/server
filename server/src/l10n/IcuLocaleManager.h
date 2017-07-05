#ifndef BEEEON_ICU_LOCALE_MANAGER_H
#define BEEEON_ICU_LOCALE_MANAGER_H

#include <map>

#include "l10n/IcuLocaleImpl.h"
#include "l10n/Locale.h"
#include "l10n/LocaleManager.h"
#include "util/Loggable.h"

namespace BeeeOn {

class IcuLocaleManager : public LocaleManager, Loggable {
public:
	IcuLocaleManager();

	Locale parse(const std::string &input) override;

	void setDefaultLocale(const std::string &locale);
	void setBundleName(const std::string &name);
	void setResourceDir(const std::string &dir);

	/**
	 * Setup the localization environment to use the uci library.
	 */
	void setup();

protected:
	Poco::SharedPtr<IcuLocaleImpl> determine(
			const IcuLocaleImpl &locale) const;
	void installDefaultLocale(const IcuLocaleImpl &locale);

private:
	std::string m_bundleName;
	std::string m_defaultLocale;
	Poco::SharedPtr<IcuLocaleImpl> m_cachedDefaultLocale;
	typedef std::map<std::string, Poco::SharedPtr<IcuLocaleImpl>> IcuLocaleMap;
	IcuLocaleMap m_availableLocales;
};

}

#endif
