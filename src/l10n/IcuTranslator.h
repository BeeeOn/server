#ifndef BEEEON_ICU_TRANSLATOR_H
#define BEEEON_ICU_TRANSLATOR_H

#include <unicode/resbund.h>

#include <Poco/SharedPtr.h>

#include "l10n/Translator.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * Utilize icu::ResourceBundle and icu::MessageFormat for localization
 * of messages. Each message is identified by a key and accepts a number
 * of arguments. The arguments are formatted into the message matching
 * the key and translator's locale. The formatting rules are described
 * at the following site:
 *
 *  http://icu-project.org/apiref/icu4c/classicu_1_1MessageFormat.html
 */
class IcuTranslator : public Translator, Loggable {
public:
	IcuTranslator(Poco::SharedPtr<icu::ResourceBundle> bundle);

protected:
	std::string formatImpl(
		const std::string &key,
		const std::vector<Poco::Dynamic::Var> &args) override;

	std::string lookupAndApplyArgs(
		const std::string &originalKey,
		std::vector<std::string>::const_iterator current,
		const std::vector<std::string>::const_iterator end,
		const icu::ResourceBundle &bundle,
		const std::vector<Poco::Dynamic::Var> &args);

	std::string applyArgs(
		const icu::ResourceBundle &bundle,
		const std::vector<Poco::Dynamic::Var> &args);

	void handleError(
		const UErrorCode error,
		const std::string &name,
		const std::string &locale) const;

	void handleWarning(
		const UErrorCode error,
		const std::string &name,
		const std::string &locale) const;

	std::string typeName(const UResType type) const;

private:
	Poco::SharedPtr<icu::ResourceBundle> m_bundle;
};

class IcuTranslatorFactory : public TranslatorFactory {
public:
	void setBundleName(const std::string &name);

	Translator *create(
		const Locale &locale,
		const std::string &name = "") override;

private:
	std::string m_bundleName;
};

}

#endif
