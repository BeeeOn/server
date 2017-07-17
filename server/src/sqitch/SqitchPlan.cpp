#include <Poco/Exception.h>
#include <Poco/FileStream.h>
#include <Poco/FileStreamFactory.h>
#include <Poco/Path.h>
#include <Poco/SharedPtr.h>

#include "sqitch/SqitchPlan.h"
#include "sqitch/SqitchPlanParser.h"

#include "util/Loggable.h"
#include <Poco/Logger.h>

using namespace std;
using namespace Poco;
using namespace BeeeOn;

SqitchPlan::SqitchPlan()
{
}

void SqitchPlan::load(const string &source)
{
	FileStreamFactory factory;
	SharedPtr<istream> in = factory.open(Path(source));

	load(*in, source);
}

void SqitchPlan::load(istream &in, const string &source)
{
	SqitchPlanParser parser(source);

	SqitchLine::Ptr line = parser.next(in);
	
	for (; !line.isNull(); line = parser.next(in))
		append(line);
}

void SqitchPlan::append(SqitchLine::Ptr line)
{
	SqitchPragma::Ptr pragma = line.cast<SqitchPragma>();
	
	if (!pragma.isNull()) {
		appendPragma(pragma);
		return;
	}

	SqitchChange::Ptr change = line.cast<SqitchChange>();
	
	if (!change.isNull()) {
		appendChange(change);
		return;
	}

	SqitchTag::Ptr tag = line.cast<SqitchTag>();

	if (!tag.isNull()) {
		appendTag(tag);
		return;
	}

	appendNote(line);
}

void SqitchPlan::appendPragma(SqitchPragma::Ptr pragma)
{
	if (pragma->key() == "project")
		m_project = pragma->value();
	else if (pragma->key() == "syntax-version")
		m_version = pragma->value();
	else if (pragma->key() == "uri")
		m_uri = URI(pragma->value());
}

void SqitchPlan::appendChange(SqitchChange::Ptr change)
{
	for (const auto &dep : change->dependencies()) {
		auto it = rfind(dep);

		if (it == m_line.rend()) {
			throw NotFoundException(
				"missing dependency '"
				+ dep.toString()
				+ "' for change '"
				+ change->name()
				+ "'"
			);
		}
	}

	m_line.emplace_back(change);
}

void SqitchPlan::appendTag(SqitchTag::Ptr tag)
{
	if (m_tags.find(tag->name()) != m_tags.end())
		throw ExistsException("duplicate tag: " + tag->name());

	m_line.emplace_back(tag);
	m_tags.emplace(tag->name());
}

void SqitchPlan::appendNote(SqitchLine::Ptr note)
{
	m_line.push_back(note);
}

list<SqitchLine::Ptr>::const_reverse_iterator SqitchPlan::rfindTag(const string &name) const
{
	for (auto it = m_line.rbegin(); it != m_line.rend(); ++it) {
		SqitchTag::Ptr tag = it->cast<SqitchTag>();	

		if (tag.isNull())
			continue;

		if (tag->name() == name)
			return it;
	}

	return m_line.rend();
}

list<SqitchLine::Ptr>::const_reverse_iterator SqitchPlan::rfind(const SqitchRef &ref) const
{
	if (ref.project(m_project) != m_project)
		return m_line.rend();

	if (!ref.tag().empty()) {
		if (m_tags.find(ref.tag()) == m_tags.end())
			return m_line.rend();
	}

	list<SqitchLine::Ptr>::const_reverse_iterator it;
	if (!ref.tag().empty())
		it = rfindTag(ref.tag());
	else
		it = m_line.rbegin();

	for (; it != m_line.rend(); ++it) {
		SqitchChange::Ptr change = it->cast<SqitchChange>();

		if (change.isNull())
			continue;

		if (change->name() == ref.name())
			return it;
	}

	return m_line.rend();
}

string SqitchPlan::project() const
{
	return m_project;
}

string SqitchPlan::version() const
{
	return m_version;
}

URI SqitchPlan::uri() const
{
	return m_uri;
}
