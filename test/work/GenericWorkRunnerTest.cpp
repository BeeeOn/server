#include <Poco/Thread.h>

#include <cppunit/extensions/HelperMacros.h>

#include "cppunit/BetterAssert.h"
#include "work/GenericWorkRunner.h"
#include "work/WorkExecutor.h"
#include "work/WorkBackup.h"
#include "work/WorkLockManager.h"
#include "work/WorkScheduler.h"
#include "work/WorkSuspendThrowable.h"

using namespace Poco;

namespace BeeeOn {

class GenericWorkRunnerTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(GenericWorkRunnerTest);
	CPPUNIT_TEST(testSuspendExecution);
	CPPUNIT_TEST(testEventSuspendExecution);
	CPPUNIT_TEST(testFinishExecution);
	CPPUNIT_TEST(testFailedExecution);
	CPPUNIT_TEST(testConcurrentExecution);
	CPPUNIT_TEST_SUITE_END();
public:
	void testSuspendExecution();
	void testEventSuspendExecution();
	void testFinishExecution();
	void testFailedExecution();
	void testConcurrentExecution();
};

CPPUNIT_TEST_SUITE_REGISTRATION(GenericWorkRunnerTest);

/**
 * Always suspend for 10 us.
 */
class SuspendingWorkExecutor : public WorkExecutor {
public:
	bool accepts(const Work::Ptr) const override
	{
		return true;
	}

	void execute(Work::Ptr) override
	{
		throw WorkSuspendThrowable(10);
	}
};

/**
 * Always suspend to wait for an event.
 */
class EventSuspendingWorkExecutor : public WorkExecutor {
public:
	bool accepts(const Work::Ptr) const override
	{
		return true;
	}

	void execute(Work::Ptr) override
	{
		throw WorkSuspendForEventThrowable();
	}
};

/**
 * Always finish the work.
 */
class FinishingWorkExecutor : public WorkExecutor {
public:
	bool accepts(const Work::Ptr) const override
	{
		return true;
	}

	void execute(Work::Ptr) override
	{
	}
};

/**
 * Always fail to execute with RuntimeException.
 */
class FailingWorkExecutor : public WorkExecutor {
public:
	bool accepts(const Work::Ptr) const override
	{
		return true;
	}

	void execute(Work::Ptr) override
	{
		throw Poco::RuntimeException("failed");
	}
};

class TestWorkScheduler : public WorkScheduler {
public:
	void schedule(Work::Ptr) override
	{
		m_schedule += 1;
	}

	void wakeup(Work::Ptr) override
	{
		m_wakeup += 1;
	}

	void cancel(Work::Ptr) override
	{
		m_cancel += 1;
	}

	void notify() override
	{
		m_notify += 1;
	}

	int m_schedule = 0;
	int m_wakeup = 0;
	int m_cancel = 0;
	int m_notify = 0;
};

class TestWorkBackup : public WorkBackup {
public:
	void store(Work::Ptr, bool) override
	{
		m_store += 1;
	}

	void loadScheduled(std::vector<Work::Ptr> &) override
	{
	}

	int m_store = 0;
};

void GenericWorkRunnerTest::testSuspendExecution()
{
	TestWorkScheduler scheduler;
	WorkLockManager manager(1, 1);
	GenericWorkRunner *runner(new GenericWorkRunner(scheduler, manager));

	TestWorkBackup backup;
	SuspendingWorkExecutor executor;
	Work::Ptr work(new Work);

	runner->setExecutor(&executor);
	runner->setWork(work);
	runner->setBackup(&backup);

	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_schedule);
	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_wakeup);
	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_cancel);
	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_notify);
	CPPUNIT_ASSERT_EQUAL(0, backup.m_store);
	CPPUNIT_ASSERT(!Work::timestampValid(work->suspended()));
	CPPUNIT_ASSERT(!Work::timestampValid(work->finished()));
	CPPUNIT_ASSERT_EQUAL(Work::STATE_IDLE, work->state());

	runner->run();

	CPPUNIT_ASSERT_EQUAL(1, scheduler.m_schedule);
	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_wakeup);
	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_cancel);
	CPPUNIT_ASSERT_EQUAL(1, scheduler.m_notify);
	CPPUNIT_ASSERT_EQUAL(1, backup.m_store);
	CPPUNIT_ASSERT(Work::timestampValid(work->suspended()));
	CPPUNIT_ASSERT(!Work::timestampValid(work->finished()));
	CPPUNIT_ASSERT_EQUAL(Work::STATE_EXECUTED, work->state());
}

void GenericWorkRunnerTest::testEventSuspendExecution()
{
	TestWorkScheduler scheduler;
	WorkLockManager manager(1, 1);
	GenericWorkRunner *runner(new GenericWorkRunner(scheduler, manager));

	TestWorkBackup backup;
	EventSuspendingWorkExecutor executor;
	Work::Ptr work(new Work);

	runner->setExecutor(&executor);
	runner->setWork(work);
	runner->setBackup(&backup);

	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_schedule);
	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_wakeup);
	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_cancel);
	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_notify);
	CPPUNIT_ASSERT_EQUAL(0, backup.m_store);
	CPPUNIT_ASSERT(!Work::timestampValid(work->suspended()));
	CPPUNIT_ASSERT(!Work::timestampValid(work->finished()));
	CPPUNIT_ASSERT_EQUAL(Work::STATE_IDLE, work->state());

	runner->run();

	CPPUNIT_ASSERT_EQUAL(1, scheduler.m_schedule);
	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_wakeup);
	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_cancel);
	CPPUNIT_ASSERT_EQUAL(1, scheduler.m_notify);
	CPPUNIT_ASSERT_EQUAL(1, backup.m_store);
	CPPUNIT_ASSERT(Work::timestampValid(work->suspended()));
	CPPUNIT_ASSERT(!Work::timestampValid(work->finished()));
	CPPUNIT_ASSERT_EQUAL(Work::STATE_EXECUTED, work->state());
}

void GenericWorkRunnerTest::testFinishExecution()
{
	TestWorkScheduler scheduler;
	WorkLockManager manager(1, 1);
	GenericWorkRunner *runner(new GenericWorkRunner(scheduler, manager));

	TestWorkBackup backup;
	FinishingWorkExecutor executor;
	Work::Ptr work(new Work);

	runner->setExecutor(&executor);
	runner->setWork(work);
	runner->setBackup(&backup);

	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_schedule);
	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_wakeup);
	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_cancel);
	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_notify);
	CPPUNIT_ASSERT_EQUAL(0, backup.m_store);
	CPPUNIT_ASSERT(!Work::timestampValid(work->suspended()));
	CPPUNIT_ASSERT(!Work::timestampValid(work->finished()));
	CPPUNIT_ASSERT_EQUAL(Work::STATE_IDLE, work->state());

	runner->run();

	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_schedule);
	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_wakeup);
	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_cancel);
	CPPUNIT_ASSERT_EQUAL(1, scheduler.m_notify);
	CPPUNIT_ASSERT_EQUAL(2, backup.m_store);
	CPPUNIT_ASSERT(!Work::timestampValid(work->suspended()));
	CPPUNIT_ASSERT(Work::timestampValid(work->finished()));
	CPPUNIT_ASSERT_EQUAL(Work::STATE_FINISHED, work->state());
}

void GenericWorkRunnerTest::testFailedExecution()
{
	TestWorkScheduler scheduler;
	WorkLockManager manager(1, 1);
	GenericWorkRunner *runner(new GenericWorkRunner(scheduler, manager));

	TestWorkBackup backup;
	FailingWorkExecutor executor;
	Work::Ptr work(new Work);

	runner->setExecutor(&executor);
	runner->setWork(work);
	runner->setBackup(&backup);

	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_schedule);
	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_wakeup);
	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_cancel);
	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_notify);
	CPPUNIT_ASSERT_EQUAL(0, backup.m_store);
	CPPUNIT_ASSERT(!Work::timestampValid(work->suspended()));
	CPPUNIT_ASSERT(!Work::timestampValid(work->finished()));
	CPPUNIT_ASSERT_EQUAL(Work::STATE_IDLE, work->state());

	runner->run();

	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_schedule);
	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_wakeup);
	CPPUNIT_ASSERT_EQUAL(0, scheduler.m_cancel);
	CPPUNIT_ASSERT_EQUAL(1, scheduler.m_notify);
	CPPUNIT_ASSERT_EQUAL(2, backup.m_store);
	CPPUNIT_ASSERT(!Work::timestampValid(work->suspended()));
	CPPUNIT_ASSERT(Work::timestampValid(work->finished()));
	CPPUNIT_ASSERT_EQUAL(Work::STATE_FAILED, work->state());
}

class ConcurrentWork : public Work {
public:
	ConcurrentWork()
	{
	}

	Poco::FastMutex m_testLock;
};

class ConcurrentExecutor : public WorkExecutor, public Poco::Runnable {
public:
	ConcurrentExecutor(WorkScheduler &scheduler, WorkLockManager &lockManager):
		m_success(false),
		m_runner(new GenericWorkRunner(scheduler, lockManager))
	{
	}

	void run() override
	{
		m_runner->run();
	}

	bool accepts(const Work::Ptr) const override
	{
		return true;
	}

	void execute(Work::Ptr work) override
	{
		m_success = work.cast<ConcurrentWork>()->m_testLock.tryLock();

		// Create quite a long time-window to allow other executors
		// to execute. The m_testLock must never be locked twice.
		// Otherwise, it means that the Work is executed in two
		// threads simultaneously which is wrong.
		Thread::sleep(50);

		if (m_success)
			work.cast<ConcurrentWork>()->m_testLock.unlock();
	}

	bool m_success;
	GenericWorkRunner *m_runner;
};

void GenericWorkRunnerTest::testConcurrentExecution()
{
	WorkLockManager lockManager(15, 15);
	Work::Ptr work(new ConcurrentWork);

	TestWorkScheduler scheduler0;
	TestWorkBackup repository0;
	ConcurrentExecutor executor0(scheduler0, lockManager);
	executor0.m_runner->setWork(work);
	executor0.m_runner->setExecutor(&executor0);
	executor0.m_runner->setBackup(&repository0);
	Thread t0;

	TestWorkScheduler scheduler1;
	TestWorkBackup repository1;
	ConcurrentExecutor executor1(scheduler1, lockManager);
	executor1.m_runner->setWork(work);
	executor1.m_runner->setExecutor(&executor1);
	executor1.m_runner->setBackup(&repository1);
	Thread t1;

	TestWorkScheduler scheduler2;
	TestWorkBackup repository2;
	ConcurrentExecutor executor2(scheduler2, lockManager);
	executor2.m_runner->setWork(work);
	executor2.m_runner->setExecutor(&executor2);
	executor2.m_runner->setBackup(&repository2);
	Thread t2;

	t2.start(executor2);
	t1.start(executor1);
	t0.start(executor0);

	t0.join();
	t1.join();
	t2.join();

	CPPUNIT_ASSERT(executor0.m_success);
	CPPUNIT_ASSERT(executor1.m_success);
	CPPUNIT_ASSERT(executor2.m_success);
}

}
