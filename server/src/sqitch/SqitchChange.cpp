#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/Exception.h>

#include "sqitch/SqitchChange.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

SqitchChange::SqitchChange(unsigned int line):
	SqitchEntry(line)
{
}

void SqitchChange::setOperation(Operation op)
{
	m_operation = op;
}

SqitchChange::Operation SqitchChange::operation() const
{
	return m_operation;
}

bool SqitchChange::addDependency(const SqitchRef &ref)
{
	if (m_conflicts.find(ref) != m_conflicts.end()) {
		throw ExistsException(
			"dependency cannot be a conflict at the same time: " + ref.toString());
	}

	return m_dependencies.emplace(ref).second;
}

set<SqitchRef> SqitchChange::dependencies() const
{
	return m_dependencies;
}

bool SqitchChange::addConflict(const SqitchRef &ref)
{
	if (m_dependencies.find(ref) != m_dependencies.end()) {
		throw ExistsException(
			"conflict cannot be a dependency at the same time: " + ref.toString());
	}

	return m_conflicts.emplace(ref).second;
}

set<SqitchRef> SqitchChange::conflicts() const
{
	return m_conflicts;
}

string SqitchChange::toString() const
{
	string result = name() + " ";

	if (!dependencies().empty() || !conflicts().empty()) {
		result += "[";

		for (const auto &d : dependencies())
			result += d.toString() + " ";

		for (const auto &c : conflicts())
			result += c.toString() + " ";

		result.back() = ']';
		result += " ";
	}

	DateTimeFormatter::append(
		result,
		timestamp(),
		DateTimeFormat::ISO8601_FORMAT
	);

	result += " ";
	result += author();
	result += " <";
	result += authorEmail();
	result += ">";

	if (!note().empty()) {
		result += " # ";
		result += note();
	}

	return result;
}
