#pragma once

#include <string>
#include <list>

#include <Poco/Path.h>

#include "dao/poco/PocoDBInitializer.h"
#include "util/Loggable.h"
#include "util/PreprocessorChain.h"

namespace BeeeOn {

/**
 * @brief PocoGlobDBInitializer initializes database with a sequence
 * of individual scripts. The scripts are sorted by names, thus naming
 * convention like 0001-new-schema.sql, 0002-new-table.sql is recommended.
 *
 * Each script is first preprocessed by the set of preconfigured preprocessors.
 */
class PocoGlobDBInitializer : public PocoDBInitializer, Loggable {
public:
	PocoGlobDBInitializer();

	/**
	 * @brief Set base dir to search for patterns in.
	 */
	void setBaseDir(const std::string &dir);

	/**
	 * @brief Set the path patterns to match scripts to be executed.
	 */
	void setPatterns(const std::list<std::string> &patterns);

	/**
	 * @brief Add preprocessor to the chain what would be applied
	 * when loading scripts.
	 */
	void addPreprocessor(Preprocessor::Ptr pp);

	/**
	 * @brief Execute the loaded script.
	 */
	void initialize(Poco::Data::Session &session) override;

protected:
	void executeScript(Poco::Data::Session &session, const std::string &file) const;

private:
	Poco::Path m_baseDir;
	std::list<std::string> m_patterns;
	PreprocessorChain m_chain;
};

}
