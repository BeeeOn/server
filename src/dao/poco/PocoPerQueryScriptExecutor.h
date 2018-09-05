#pragma once

#include <string>
#include <vector>

#include "dao/poco/PocoScriptExecutor.h"
#include "util/Loggable.h"
#include "util/PreprocessorChain.h"

namespace BeeeOn {

/**
 * @brief PocoPerQueryScriptExecutor executes database script by first
 * splitting it into particular queries. Then, the individual queries
 * are executed against the database. Splitting is done without parsing
 * of queries. Each query MUST end by sequence <code>';\n'</code> or
 * <code>';<EOF>'</code>.
 *
 * The PocoPerQueryScriptExecutor might need some preprocessors to be applied
 * before the query splitting. This would allow e.g. to get rid of comments
 * or other stuff that would otherwise break the execution.
 */
class PocoPerQueryScriptExecutor : public PocoScriptExecutor, Loggable {
public:
	PocoPerQueryScriptExecutor();

	/**
	 * @brief Execute the given script.
	 */
	unsigned long execute(
		Poco::Data::Session &session,
		const std::string &script) const override;

	/**
	 * @brief Set whether to fail when the script ends with garbage
	 * (missing <code>';'</code> at the end of file).
	 */
	void setAllowGarbage(bool allow);

protected:
	/**
	 * @brief Split the given input script into queries. Each query can
	 * be multiline but it MUST end by sequence <code>';\n'</code> or
	 * <code>';<EOF>'</code>.
	 */
	std::vector<std::string> splitQueries(const std::string &script) const;

private:
	bool m_allowGarbage;
};

}
