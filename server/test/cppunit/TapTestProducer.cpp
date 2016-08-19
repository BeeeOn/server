#include <cppunit/Message.h>

#include "cppunit/TapTestProducer.h"

using namespace CppUnit;
using namespace BeeeOn;

void TapTestProducer::clearResults()
{
	std::map<int, TestFailureList>::iterator it;
	TestFailureList::iterator failures;

	for (it = m_failure.begin(); it != m_failure.end(); ++it) {
		failures = it->second.begin();

		for(; failures != it->second.end(); ++failures)
			delete *failures;
	}

	m_failure.clear();
	m_name.clear();
}

void TapTestProducer::reportSuccess(int index, const std::string &name)
{
	m_output << "ok " << index << " - " << name << std::endl;
}

void TapTestProducer::reportException(const Exception *e)
{
	if (e == NULL)
		return;

	const Message &m = e->message();

	m_output << " message: '" << m.shortDescription() << "'" << std::endl;

	for (int i = 0; i < m.detailCount(); ++i)
		m_output << " detail: '" << m.detailAt(i) << "'" << std::endl;
}

void TapTestProducer::reportSourceLine(const SourceLine &line)
{
	if (!line.isValid()) {
		m_output << "# source: unknown" << std::endl;
		return;
	}

	m_output << " file: '" << line.fileName() << "'" << std::endl;
	m_output << " line: " << line.lineNumber() << std::endl;
}

void TapTestProducer::reportFailure(int index, const TestFailureList &failures)
{
	TestFailureList::const_iterator it = failures.begin();

	for (; it != failures.end(); ++it) {
		const TestFailure *failure = *it;

		m_output << " ---" << std::endl;
		reportSourceLine(failure->sourceLine());
		reportException(failure->thrownException());
		m_output << " ..." << std::endl;

		m_output << "not ok " << index << " - "
			<< m_name[index] << std::endl;
	}
}

void TapTestProducer::reportSuite()
{
	std::map<int, std::string>::const_iterator names;
	std::map<int, TestFailureList>::const_iterator fail;

	if (m_name.empty()) // no test has been executed
		return;

	m_output << "1.." << m_name.size() << std::endl;

	names = m_name.begin();
	for (; names != m_name.end(); ++names) {
		fail = m_failure.find(names->first);

		if (fail == m_failure.end()) {
			reportSuccess(names->first, names->second);
			continue;
		}

		reportFailure(names->first, fail->second);
	}

	clearResults();
}
