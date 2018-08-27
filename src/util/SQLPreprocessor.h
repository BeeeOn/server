#pragma once

#include "util/Preprocessor.h"

namespace BeeeOn {

/**
 * @brief SQLPreprocessor applies some preprocessing rules
 * to the given query. Its purpose is mainly to shrink
 * queries by trimming unneeded lines or characters.
 *
 * The preprocessing must be done carefully to avoid dropping
 * of certain important parts of queries. This can happen when
 * using comment tokens inside quotes.
 *
 * The SQLPreprocessor implements:
 *
 * - dropping of SQL comments starting as --
 * - trimming of unnecessary whitespace until end of line
 * - dropping empty lines (i.e. lines with comments or whitespace only)
 *
 * Those three features can be turn on or off to give workaround
 * for queries that would be unexpectedly broken. Moreover, dropping
 * of empty lines does not help while debugging (we get source line
 * but the query's line numbers do not match anymore).
 */
class SQLPreprocessor : public Preprocessor {
public:
	typedef Poco::SharedPtr<SQLPreprocessor> Ptr;

	SQLPreprocessor();

	void setRemoveComments(bool remove);
	void setRemoveWhitespace(bool remove);
	void setPreserveUnneededLines(bool preserve);

	std::string process(const std::string &input) const override;

protected:
	void removeComments(std::string &line) const;

private:
	bool m_removeComments;
	bool m_removeWhitespace;
	bool m_preserveUnneededLines;
};

}
