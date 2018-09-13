#pragma once

#include <iosfwd>
#include <list>
#include <map>
#include <string>

#include <Poco/URI.h>

#include "sqitch/SqitchChange.h"
#include "sqitch/SqitchEntry.h"
#include "sqitch/SqitchLine.h"
#include "sqitch/SqitchPragma.h"
#include "sqitch/SqitchTag.h"

namespace BeeeOn {

class SqitchPlan {
public:
	SqitchPlan();

	void load(const std::string &source);
	void load(std::istream &in, const std::string &source = "<unknown>");

	std::string version() const;
	std::string project() const;
	Poco::URI uri() const;

	void append(SqitchLine::Ptr line);
	void appendPragma(SqitchPragma::Ptr pragma);
	void appendChange(SqitchChange::Ptr change);
	void appendTag(SqitchTag::Ptr tag);
	void appendNote(SqitchLine::Ptr note);

	std::list<SqitchLine::Ptr>::const_iterator begin() const
	{
		return m_line.begin();
	}

	std::list<SqitchLine::Ptr>::const_iterator end() const
	{
		return m_line.end();
	}

	std::list<SqitchLine::Ptr>::const_reverse_iterator rbegin() const
	{
		return m_line.rbegin();
	}

	std::list<SqitchLine::Ptr>::const_reverse_iterator rend() const
	{
		return m_line.rend();
	}

	std::list<SqitchLine::Ptr>::const_reverse_iterator rfind(const SqitchRef &ref) const;
	std::list<SqitchLine::Ptr>::const_reverse_iterator rfindTag(const std::string &name) const;

private:
	std::string m_project;
	std::string m_version;
	Poco::URI m_uri;
	std::list<SqitchLine::Ptr> m_line;
	std::set<std::string> m_tags;
};

}
