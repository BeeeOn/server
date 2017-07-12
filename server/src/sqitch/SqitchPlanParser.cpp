#include <istream>

#include <Poco/DateTime.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeParser.h>
#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/RegularExpression.h>
#include <Poco/String.h>
#include <Poco/StringTokenizer.h>
#include <Poco/Timestamp.h>

#include "sqitch/SqitchChange.h"
#include "sqitch/SqitchPlanParser.h"
#include "sqitch/SqitchPragma.h"
#include "sqitch/SqitchTag.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

SqitchPlanParser::SqitchPlanParser(const string &source):
	m_source(source),
	m_lineno(0)
{
}

SqitchLine::Ptr SqitchPlanParser::next(std::istream &in)
{
	SqitchLine::Ptr line;

	while (line.isNull() && !in.eof()) {
		if (!in.good())
			throw IOException("cannot read from input stream");

		string buffer;
		getline(in, buffer);
		++m_lineno;
		line = parseLine(trimInPlace(buffer));
	}

	return line;
}

string SqitchPlanParser::location() const
{
	return m_source + ":" + to_string(m_lineno);
}

SqitchLine::Ptr SqitchPlanParser::parseLine(const string &line) const
{
	if (logger().trace())
		logger().trace(line, __FILE__, __LINE__);

	if (line.empty())
		return NULL;

	if (line[0] == '%')
		return parsePragma(line);
	else if (line[0] == '@')
		return parseTag(line);
	else if (line[0] == '#')
		return parseNote(line);
	else
		return parseChange(line);
}

SqitchLine::Ptr SqitchPlanParser::parsePragma(const string &line) const
{
	// http://search.cpan.org/~dwheeler/App-Sqitch-0.999/lib/App/Sqitch/PlanParser.pm#Grammar
	RegularExpression expr(
		"^"
		"%[[:blank:]]*"
		"([^=]+)"
		"="
		"([^#]*)"
		"(#.*)?"
		"$"
	);

	RegularExpression::MatchVec m;

	if (expr.match(line, 0, m)) {
		string name;
		string value;
		string note;

		if (m.size() >= 3) {
			name = trim(line.substr(m[1].offset, m[1].length));
			value = trim(line.substr(m[2].offset, m[2].length));
		}

		if (logger().debug()) {
			logger().debug(
				"pragma: '"
				+ name
				+ "' = '"
				+ value
				+ "'",
				__FILE__, __LINE__
			);
		}

		if (m.size() > 3)
			note = extractNote(trim(line.substr(m[3].offset, m[3].length)));

		return buildPragma(name, value, note);
	}
	else {
		throw SyntaxException("invalid pragma line: '" + line + "' at " + location());
	}
}

SqitchLine::Ptr SqitchPlanParser::buildPragma(
		const string &name,
		const string &value,
		const string &note) const
{
	SqitchPragma::Ptr pragma = new SqitchPragma(m_lineno);
	pragma->setKey(name);
	pragma->setValue(value);
	pragma->setNote(note);

	return pragma;
}

SqitchLine::Ptr SqitchPlanParser::parseTag(const string &line) const
{
	RegularExpression expr(
		"^"
		"@"
		"([^[:blank:]]+)"
		"[[:blank:]]+"
		"([-+0-9TZ:]+)"
		"[[:blank:]]+"
		"([^#]+)"
		"[[:blank:]]*"
		"(#.*)?"
		"$"
	);

	RegularExpression::MatchVec m;

	if (expr.match(line, 0, m)) {
		string name;
		string time;
		string author;
		string note;

		if (m.size() >= 4) {
			name = trim(line.substr(m[1].offset, m[1].length));
			time = trim(line.substr(m[2].offset, m[2].length));
			author = trim(line.substr(m[3].offset, m[3].length));
		}

		if (logger().debug()) {
			logger().debug(
				"tag: '"
				+ name
				+ "' '"
				+ time
				+ "' '"
				+ author
				+ "'",
				__FILE__, __LINE__
			);
		}

		if (m.size() > 4)
			note = extractNote(trim(line.substr(m[4].offset, m[4].length)));

		return buildTag(name, time, author, note);
	}
	else {
		throw SyntaxException("invalid tag line: '" + line + "'");
	}
}

SqitchLine::Ptr SqitchPlanParser::buildTag(
		const string &name,
		const string &time,
		const string &author,
		const string &note) const
{
	if (name == "HEAD" || name == "ROOT")
		throw SyntaxException("reserved name cannot be a tag: '" + name + "'");

	SqitchTag::Ptr tag = new SqitchTag(m_lineno);
	buildEntry(tag, name, time, author);
	tag->setNote(note);

	return tag;
}

#define RGX_PUNCT "-!\"#$%&'()*+,./:;<=>?@\\[\\]\\\\^`{|}~"

SqitchLine::Ptr SqitchPlanParser::parseChange(const string &line) const
{
	// https://github.com/theory/sqitch/blob/master/lib/App/Sqitch/PlanParser.pm
	RegularExpression expr(
		"^"
		"([+-]|)[[:blank:]]*"
		"("
		"(?![" RGX_PUNCT "])"
		"(?:"
			"(?!"
				"[~/=%^]"
				"[[:digit:]]+"
				"(?:$|[[:blank:]])"
			")"
			"[^[:blank:]:@#]"
		")+"
		"(?<![" RGX_PUNCT "])\\b"
		")"
		"[[:blank:]]+"
		"((?:\\[[^\\]]+\\])?)"
		"[[:blank:]]*"
		"([-+0-9TZ:]+)"
		"[[:blank:]]+"
		"([^#]+)"
		"[[:blank:]]*"
		"(#.*)?"
		"$"
	);

	RegularExpression::MatchVec m;

	if (expr.match(line, 0, m)) {
		string op;
		string name;
		string deps;
		string time;
		string author;
		string note;

		if (m.size() >= 6) {
			op = trim(line.substr(m[1].offset, m[1].length));
			name = trim(line.substr(m[2].offset, m[2].length));
			deps = trim(line.substr(m[3].offset, m[3].length));
			time = trim(line.substr(m[4].offset, m[4].length));
			author = trim(line.substr(m[5].offset, m[5].length));
		}

		if (logger().debug()) {
			logger().debug(
				"change: '"
				+ name
				+ "' '"
				+ time
				+ "' '"
				+ author + "'",
				__FILE__, __LINE__
			);
		}

		if (m.size() > 6)
			note = extractNote(trim(line.substr(m[6].offset, m[6].length)));

		return buildChange(op, name, deps, time, author, note);
	}
	else {
		throw SyntaxException("invalid change line: '" + line + "'");
	}
}

SqitchLine::Ptr SqitchPlanParser::buildChange(
		const string &op,
		const string &name,
		const string &deps,
		const string &time,
		const string &author,
		const string &note) const
{
	SqitchChange::Ptr change = new SqitchChange(m_lineno);
	buildEntry(change, name, time, author);
	extractDeps(change, deps);
	change->setNote(note);

	if (op == "+" || op.empty())
		change->setOperation(SqitchChange::DEPLOY);
	else if (op == "-")
		change->setOperation(SqitchChange::REVERT);
	else
		throw IllegalStateException("unexpected change operator: '" + op + "'");

	return change;
}

void SqitchPlanParser::buildEntry(
		SqitchEntry::Ptr entry,
		const string &name,
		const string &time,
		const string &author) const
{
	Timestamp timestamp = extractTimestamp(time);
	string authorName;
	string authorEmail;
	extractAuthor(author, authorName, authorEmail);

	entry->setName(name);
	entry->setTimestamp(timestamp);
	entry->setAuthor(authorName);
	entry->setAuthorEmail(authorEmail);
}

SqitchLine::Ptr SqitchPlanParser::parseNote(const string &line) const
{
	SqitchLine::Ptr note = new SqitchLine(m_lineno);
	note->setNote(extractNote(line));
	return note;
}

string SqitchPlanParser::extractNote(const string &line) const
{
	RegularExpression expr(
		"^"
		"#[[:blank:]]*([^[:blank:]].*)?"
		"$"
	);

	RegularExpression::MatchVec m;

	if (expr.match(line, 0, m)) {
		if (m.size() >= 1)
			return trim(line.substr(m[1].offset, m[1].length));
	}

	return "";
}

Timestamp SqitchPlanParser::extractTimestamp(const string &text) const
{
	int ignore;
	DateTime dt = DateTimeParser::parse(
			DateTimeFormat::ISO8601_FORMAT, text, ignore);

	return dt.timestamp();
}

void SqitchPlanParser::extractAuthor(
		const string &text,
		string &author,
		string &email) const
{
	RegularExpression expr(
		"^([^<]+)[[:blank:]]+<([^>]+@[^>]+)>$"
	);

	RegularExpression::MatchVec m;

	if (expr.match(text, 0, m)) {
		author = text.substr(m[1].offset, m[1].length);
		email = text.substr(m[2].offset, m[2].length);
		return;
	}

	throw SyntaxException("invalid author specification: '" + text + "'");
}

void SqitchPlanParser::extractDeps(
	SqitchChange::Ptr change,
	const string &deps) const
{
	if (deps.empty())
		return;

	if (deps.front() != '[' || deps.back() != ']') {
		throw SyntaxException(
			"invalid dependency list, missing '[' or ']': " + deps);
	}

	string names = deps.substr(1); // drop '['
	names.pop_back(); // drop ']'

	StringTokenizer tokenizer(names, " \t",
		StringTokenizer::TOK_TRIM | StringTokenizer::TOK_IGNORE_EMPTY);

	for (const auto &token : tokenizer) {
		if (logger().debug()) {
			logger().debug("dependency: '" + token + "'",
					__FILE__, __LINE__);
		}

		if (token.front() == '!') {
			if (change->addConflict(token.substr(1)))
				continue;

			logger().warning("duplicate conflict: '" + token + "'",
					__FILE__, __LINE__);
		}
		else {
			if (change->addDependency(token))
				continue;

			logger().warning("duplicate dependency: " + token,
					__FILE__, __LINE__);
		}
	}
}
