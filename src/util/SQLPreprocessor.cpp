#include <Poco/RegularExpression.h>
#include <Poco/String.h>
#include <Poco/StringTokenizer.h>

#include "util/SQLPreprocessor.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

static const RegularExpression SQL_COMMENT("--.*$");
static const RegularExpression SQL_QUOTE("['\"]");

SQLPreprocessor::SQLPreprocessor():
	m_removeComments(true),
	m_removeWhitespace(true),
	m_preserveUnneededLines(false)
{
}

void SQLPreprocessor::setRemoveComments(bool remove)
{
	m_removeComments = remove;
}

void SQLPreprocessor::setRemoveWhitespace(bool remove)
{
	m_removeWhitespace = remove;
}

void SQLPreprocessor::setPreserveUnneededLines(bool preserve)
{
	m_preserveUnneededLines = preserve;
}

void SQLPreprocessor::removeComments(string &line) const
{
	RegularExpression::Match comment;
	if (!SQL_COMMENT.match(line, comment))
		return;

	RegularExpression::Match quote;
	if (SQL_QUOTE.match(line, quote)) {
		/*
		 * Run away, we are lazy to do full syntax analysis.
		 * This way, we can easily break the query.
		 */
		if (quote.offset < comment.offset)
			return;
	}

	line = line.substr(0, comment.offset);
}

string SQLPreprocessor::process(const string &input) const
{
	StringTokenizer lines(input, "\n");
	string result;

	for (auto it = lines.begin(); it != lines.end(); ++it) {
		string line = *it;

		if (m_removeComments)
			removeComments(line);

		if (m_removeWhitespace)
			trimRightInPlace(line);

		if (!m_preserveUnneededLines && line.empty())
			continue;

		result += line;

		if ((it + 1) != lines.end())
			result += "\n";
	}

	if (!m_preserveUnneededLines) {
		if (!result.empty() && result.back() == '\n')
			result.pop_back();
	}

	return result;
}
