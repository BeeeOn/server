#include <Poco/Exception.h>
#include <Poco/RegularExpression.h>

#include "sqitch/SqitchRef.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

SqitchRef::SqitchRef(const string &input)
{
	parse(input);
}

SqitchRef::SqitchRef(
		const string &project,
		const string &name,
		const string &tag):
	m_project(project),
	m_name(name),
	m_tag(tag)
{
}

void SqitchRef::parse(const string &input)
{
	if (input.empty())
		throw SyntaxException("empty sqitch reference");

	RegularExpression expr(
		"^((?:[^:]+:)?)([^:@]+)((?:@[^:@]+)?)$"
	);

	RegularExpression::MatchVec m;

	if (expr.match(input, 0, m) != 4)
		throw SyntaxException("invalid sqitch ref: '" + input + "'");

	m_project = input.substr(m[1].offset, m[1].length);
	m_name = input.substr(m[2].offset, m[2].length);
	m_tag = input.substr(m[3].offset, m[3].length);

	// drop ending ':' if any
	if (!m_project.empty())
		m_project = m_project.substr(0, m_project.size() - 1);

	// drop starting '@' if any
	if (!m_tag.empty())
		m_tag = m_tag.substr(1);
}

string SqitchRef::project(const string &current) const
{
	if (m_project.empty())
		return current;

	return m_project;
}

string SqitchRef::name() const
{
	return m_name;
}

string SqitchRef::tag(const string &last) const
{
	if (m_tag.empty())
		return last;

	return m_tag;
}

string SqitchRef::toString() const
{
	string result = name();

	if (!project().empty())
		result = project() + ":" + result;
	if (!tag().empty())
		result += "@" + tag();

	return result;
}

SqitchRef SqitchRef::forProject(const string &project) const
{
	return SqitchRef(project, name(), tag());
}

bool SqitchRef::equals(const SqitchRef &other) const
{
	return project(other.m_project) == other.project(m_project)
		&& m_name == other.m_name
		&& m_tag == other.m_tag;
}

bool SqitchRef::lessThen(const SqitchRef &other) const
{
	if (project(other.m_project) < other.project(m_project))
		return true;

	if (project(other.m_project) > other.project(m_project))
		return false;

	if (m_tag < other.m_tag)
		return true;

	if (m_tag > other.m_tag)
		return false;

	return m_name < other.m_name;
}
