#ifndef BEEEON_HTTP_LOCALE_EXTRACTOR_H
#define BEEEON_HTTP_LOCALE_EXTRACTOR_H

#include <Poco/SharedPtr.h>

#include "l10n/Locale.h"
#include "l10n/LocaleManager.h"
#include "util/Loggable.h"

namespace BeeeOn {

class HTTPLocaleExtractor : public Loggable {
public:
	HTTPLocaleExtractor();

	void setLocaleManager(Poco::SharedPtr<LocaleManager> manager);

	/**
	 * Extract the most suitable locale from the given header
	 * contents obtained most probably from the HTTP header
	 * Accept-Language.
	 */
	Locale extract(const std::string &header);

	/**
	 * Parse weight and return its representation in range 0..1000.
	 * Thus 0 is returned 0, 1 is returned 1000. For invalid inputs,
	 * the result is -1.
	 */
	static int parseWeight(const std::string &input);

private:
	class HTTPLocale {
	public:
		HTTPLocale();
		
		void setUnresolvedLocale(const std::string &locale);
		const std::string &unresolvedLocale() const;

		void setWeight(const unsigned int weight);
		unsigned int weight() const;

		bool operator <(const HTTPLocale &other) const
		{
			return betterWeight(other);
		}

		bool betterWeight(const HTTPLocale &other) const;

	private:
		std::string m_unresolvedLocale;
		unsigned int m_weight;
	};

	Poco::SharedPtr<LocaleManager> m_localeManager;
};

}

#endif
