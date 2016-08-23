#ifndef BEEEON_TAP_TEST_PRODUCER_H
#define BEEEON_TAP_TEST_PRODUCER_H

#include <iostream>
#include <map>
#include <vector>
#include <cppunit/TestListener.h>
#include <cppunit/TestFailure.h>
#include <cppunit/Exception.h>
#include <cppunit/SourceLine.h>
#include <cppunit/Test.h>

namespace BeeeOn {

/**
 * Plugin to CppUnit that outputs the test results in
 * the TAP format:
 *
 *  https://testanything.org/tap-version-13-specification.html
 *
 * The output is printed after all tests are executed thus
 * measuring time of single tests is not possible by the
 * target TAP consumer. This is simplifies the implementation
 * as the CppUnit has hierachical tests structure.
 */
class TapTestProducer : public CppUnit::TestListener {
public:
	typedef std::vector<CppUnit::TestFailure *> TestFailureList;

	TapTestProducer():
		m_output(std::cout),
		m_success(true),
		m_current(1),
		m_stackDepth(0)
	{
	}

	TapTestProducer(std::ostream &output):
		m_output(output),
		m_success(true),
		m_current(1),
		m_stackDepth(0)
	{
	}

	~TapTestProducer()
	{
		clearResults();
	}

private:
	/**
	 * Clear all results (free memory if necessary).
	 */
	void clearResults();

	/**
	 * Report that the test of the given index has succeeded.
	 */
	void reportSuccess(int index, const std::string &name);

	/**
	 * Report details of an exception thrown by a test.
	 */
	void reportException(const CppUnit::Exception *e);

	/**
	 * Report source file and line of an event.
	 */
	void reportSourceLine(const CppUnit::SourceLine &line);

	/**
	 * Report that the test of the given index has failed.
	 */
	void reportFailure(int index, const TestFailureList &failures);

	/**
	 * Outputs the TAP report after a suite has finished.
	 */
	void reportSuite();

public:
	void startSuite(CppUnit::Test *suite)
	{
		m_stackDepth += 1;
	}

	void endSuite(CppUnit::Test *suite)
	{
		m_stackDepth -= 1;

		if (m_stackDepth > 0) // report the top-level suite only
			return;

		reportSuite();
	}

	void startTest(CppUnit::Test *test)
	{
		m_name[m_current] = test->getName();
	}

	void endTest(CppUnit::Test *test)
	{
		m_current += 1;
	}

	void addFailure(const CppUnit::TestFailure &failure)
	{
		m_failure[m_current].push_back(failure.clone());
		m_success = false;
	}

	bool wasSuccessful() const
	{
		return m_success;
	}

private:
	/**
	 * Output stream to report to.
	 */
	std::ostream &m_output;
	/**
	 * Collection of all executed tests. Maps a test index to
	 * its name. The index is an incrementing number starting*
	 * at 1.
	 */
	std::map<int, std::string> m_name;
	/**
	 * Collection of all failures. Maps a test index to its
	 * list of failures.
	 */
	std::map<int, TestFailureList> m_failure;
	/**
	 * Index of the current test being executed.
	 */
	unsigned int m_current;
	/**
	 * Position in the test groups hierarchy. The more deeper
	 * in the hierarchy we are the greater is this value.
	 */
	unsigned int m_stackDepth;
	/**
	 * Overall test suite success. If at least one test fails
	 * the success is false.
	 */
	bool m_success;
};

}

#endif
