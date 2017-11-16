#include <list>

#include <Poco/AtomicCounter.h>
#include <Poco/Mutex.h>
#include <Poco/Runnable.h>
#include <Poco/Thread.h>
#include <Poco/Timestamp.h>

#include <cppunit/extensions/HelperMacros.h>

#include "cppunit/BetterAssert.h"

#include "work/BasicProcessor.h"
#include "work/Work.h"
#include "work/WorkExecutor.h"
#include "work/WorkLockManager.h"

using namespace Poco;

namespace BeeeOn {

class TestableBasicProcessor;

class BasicProcessorTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(BasicProcessorTest);
	CPPUNIT_TEST(testNoMoreThreads);
	CPPUNIT_TEST(testDispatchInterruptible);
	CPPUNIT_TEST(testDispatchFinishedWork);
	CPPUNIT_TEST(testNoMatchingExecutor);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

	void testNoMoreThreads();
	void testDispatchInterruptible();
	void testDispatchFinishedWork();
	void testNoMatchingExecutor();

private:
	TestableBasicProcessor *m_processor;
	WorkLockManager::Ptr m_lockManager;
};

CPPUNIT_TEST_SUITE_REGISTRATION(BasicProcessorTest);

class TestableBasicProcessor : public BasicProcessor {
public:
	using BasicProcessor::dispatch;
	using BasicProcessor::executeInThread;
	using BasicProcessor::pool;
};

class YieldingRunnable : public Poco::Runnable {
public:
	void run() override {
		while (!m_done)
			Thread::yield();
	}

	AtomicCounter m_done;
};

class NotAcceptingExecutor : public WorkExecutor {
public:
	bool accepts(const Work::Ptr) const override
	{
		return false;
	}

	void execute(Work::Ptr) override
	{
		throw Poco::AssertionViolationException("this must never be called");
	}
};

void BasicProcessorTest::setUp()
{
	m_lockManager = new WorkLockManager(31, 63);
	m_processor = new TestableBasicProcessor();
	m_processor->setLockManager(m_lockManager);
}

void BasicProcessorTest::tearDown()
{
	delete m_processor;
}

/**
 * Test executeInThread returns false when no more threads
 * are available.
 */
void BasicProcessorTest::testNoMoreThreads()
{
	m_processor->setMinThreads(1);
	m_processor->setMaxThreads(1);

	YieldingRunnable runnable0;
	YieldingRunnable runnable1;

	CPPUNIT_ASSERT(m_processor->executeInThread(runnable0, "test0"));
	CPPUNIT_ASSERT(!m_processor->executeInThread(runnable1, "test1"));

	runnable0.m_done = 1;
	m_processor->pool().joinAll();
}

/**
 * Test that when the dispatch() method starts blocking due to an empty
 * queue, it can be interrupted by calling notify().
 */
void BasicProcessorTest::testDispatchInterruptible()
{
	NotAcceptingExecutor executor;
	m_processor->registerExecutor(&executor);

	Timestamp now;

	Thread t;
	t.startFunc(
		[&]() {
			Thread::sleep(100);
			m_processor->notify();
		}
	);

	CPPUNIT_ASSERT(now.elapsed() < 100000);
	m_processor->dispatch();
	CPPUNIT_ASSERT(now.elapsed() >= 100000);

	t.join();
}

/**
 * Assure there is no unexpected behaviour when
 * a finished work is encountered in the queue.
 */
void BasicProcessorTest::testDispatchFinishedWork()
{
	Work::Ptr work(new Work);

	m_processor->schedule(work);

	work->setFinished(work->created());
	CPPUNIT_ASSERT(Work::timestampValid(work->finished()));
	work->setState(Work::STATE_FINISHED);

	m_processor->dispatch();
}

void BasicProcessorTest::testNoMatchingExecutor()
{
	NotAcceptingExecutor executor;

	m_processor->registerExecutor(&executor);

	Work::Ptr work(new Work(WorkID::parse("8028bf15-51f7-4e12-af11-fc8cafc18917")));

	CPPUNIT_ASSERT(work->state() == Work::STATE_IDLE);

	m_processor->schedule(work);
	m_processor->dispatch();

	CPPUNIT_ASSERT(work->state() == Work::STATE_FAILED);
}

}
