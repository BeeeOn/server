#pragma once

#include <iosfwd>
#include <string>

#include <Poco/URI.h>

#include "sqitch/SqitchLine.h"
#include "util/Loggable.h"

namespace BeeeOn {

class SqitchPlanParser : Loggable {
public:
	SqitchPlanParser(const std::string &source = "<unknown>");

	SqitchLine::Ptr next(std::istream &in);

private:
	std::string location() const;
	SqitchLine::Ptr parseLine(const std::string &line) const;

	SqitchLine::Ptr parsePragma(const std::string &line) const;
	SqitchLine::Ptr buildPragma(
		const std::string &name,
		const std::string &value,
		const std::string &note) const;

	SqitchLine::Ptr parseTag(const std::string &line) const;
	SqitchLine::Ptr buildTag(
		const std::string &name,
		const std::string &time,
		const std::string &author,
		const std::string &note) const;

	SqitchLine::Ptr parseChange(const std::string &line) const;
	SqitchLine::Ptr buildChange(
		const std::string &op,
		const std::string &name,
		const std::string &deps,
		const std::string &time,
		const std::string &author,
		const std::string &note) const;

	void buildEntry(
		SqitchEntry::Ptr entry,
		const std::string &name,
		const std::string &time,
		const std::string &author) const;

	SqitchLine::Ptr parseNote(const std::string &line) const;
	std::string extractNote(const std::string &line) const;

	Poco::Timestamp extractTimestamp(const std::string &text) const;

	void extractAuthor(
		const std::string &text,
		std::string &author,
		std::string &email) const;

	void extractDeps(
		SqitchChange::Ptr change,
		const std::string &deps) const;

private:
	std::string m_source;
	unsigned int m_lineno;
};

}
