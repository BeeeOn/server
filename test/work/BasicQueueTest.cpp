#include <Poco/AtomicCounter.h>
#include <Poco/Timestamp.h>
#include <Poco/Timespan.h>
#include <Poco/Timer.h>

#include <cppunit/extensions/HelperMacros.h>

#include "cppunit/BetterAssert.h"
#include "work/BasicQueue.h"
#include "work/Work.h"

using namespace Poco;

namespace BeeeOn {

class BasicQueueTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(BasicQueueTest);
	CPPUNIT_TEST(testSimpleSchedule);
	CPPUNIT_TEST(testPrioritySchedule);
	CPPUNIT_TEST(testScheduleWithSuspend);
	CPPUNIT_TEST(testScheduleIsNotDone);
	CPPUNIT_TEST(testCancelOutOfOrder);
	CPPUNIT_TEST(testCancelNonExisting);
	CPPUNIT_TEST(testCurrentlyNothing);
	CPPUNIT_TEST(testSimpleBlocking);
	CPPUNIT_TEST(testBlockingWithAsyncSchedules);
	CPPUNIT_TEST(testBlockingWithWakeup);
	CPPUNIT_TEST(testWakeupWaiting);
	CPPUNIT_TEST(testWakeupNonexisting);
	CPPUNIT_TEST_SUITE_END();
public:
	void testSimpleSchedule();
	void testPrioritySchedule();
	void testScheduleWithSuspend();
	void testScheduleIsNotDone();
	void testCancelOutOfOrder();
	void testCancelNonExisting();
	void testCurrentlyNothing();
	void testSimpleBlocking();
	void testBlockingWithAsyncSchedules();
	void testBlockingWithWakeup();
	void testWakeupWaiting();
	void testWakeupNonexisting();
};

CPPUNIT_TEST_SUITE_REGISTRATION(BasicQueueTest);

/**
 * Schedule 3 different works with different times.
 * Check they are scheduled and we can peek the
 * right one.
 */
void BasicQueueTest::testSimpleSchedule()
{
	Timestamp now;
	BasicQueue queue;

	CPPUNIT_ASSERT(queue.pop(false).isNull());
	
	Work::Ptr work0(new Work(WorkID::parse("238e8d19-f10d-4a76-afd5-96d5476aa614")));
	work0->setCreated(now - 5);
	work0->setSleepDuration(2);

	Work::Ptr work1(new Work(WorkID::parse("1fdd4021-b364-4b31-916e-61b2817da57b")));
	work1->setCreated(now - 5);
	work1->setSleepDuration(1);

	Work::Ptr work2(new Work(WorkID::parse("532cde6e-2cb1-457b-9623-fd0d4d478691")));
	work2->setCreated(now - 5);
	work2->setSleepDuration(3);

	do {
		FastMutex::ScopedLock guard(queue.lock());

		queue.pushUnlocked(work0);
		queue.pushUnlocked(work1);
		queue.pushUnlocked(work2);
	} while (0);

	// remove and test order
	CPPUNIT_ASSERT(work1 == queue.pop(false));
	CPPUNIT_ASSERT(work0 == queue.pop(false));
	CPPUNIT_ASSERT(work2 == queue.pop(false));

	CPPUNIT_ASSERT(queue.pop(false).isNull());
}

/**
 * Test scheduling when priority is used. The works must be scheduled
 * in this order:
 *
 * 1. work3 (now - 5 us, 1) - highest priority with lowest activation time
 * 2. work1 (now - 1 us, 1) - highest priority with second lowest activation time
 * 3. work2 (now, 0) - wins by priority
 * 4. work0 (now - 10, -1) - lowest activation time ever, however too low priority
 */
void BasicQueueTest::testPrioritySchedule()
{
	Timestamp now;
	BasicQueue queue;

	CPPUNIT_ASSERT(queue.pop(false).isNull());

	// This test is time based so wait until we have a good starting point
	// to not easily overflow 5 seconds (see BasicQueue::Key for details).
	const long GOOD_INTERVAL_US = BasicQueue::Key::EVERY_5SECS * 1000000;
	// 500 ms is enough to execute and not too long to delay the test too much.
	const long TOLERANCE_US = GOOD_INTERVAL_US - 500000;
	// Assure we are at the beginning of the 5 seconds interval.
	while (!(now.epochMicroseconds() % GOOD_INTERVAL_US < TOLERANCE_US))
		now.update();

	Work::Ptr work0(new Work(WorkID::parse("ad90d0c3-85b5-42a2-a2a5-8212a6bf8ee0")));
	work0->setCreated(now);
	work0->setSleepDuration(0);
	work0->setPriority(-1);

	Work::Ptr work1(new Work(WorkID::parse("0e38ecdd-009e-4a05-ab76-b0d3ffd41495")));
	work1->setCreated(now);
	work1->setSleepDuration(5);
	work1->setPriority(1);

	Work::Ptr work2(new Work(WorkID::parse("b2a43b38-3149-4cd6-96f6-89b955be5f03")));
	work2->setCreated(now);
	work2->setSleepDuration(2);

	Work::Ptr work3(new Work(WorkID::parse("c63de6d1-8b75-45b9-8b57-67cf876c1048")));
	work3->setCreated(now);
	work3->setSleepDuration(1);
	work3->setPriority(1);

	do {
		FastMutex::ScopedLock guard(queue.lock());

		queue.pushUnlocked(work0);
		queue.pushUnlocked(work1);
		queue.pushUnlocked(work2);
		queue.pushUnlocked(work3);
	} while (0);

	// remove and test order
	CPPUNIT_ASSERT(work3 == queue.pop(false));
	CPPUNIT_ASSERT(work1 == queue.pop(false));
	CPPUNIT_ASSERT(work2 == queue.pop(false));
	CPPUNIT_ASSERT(work0 == queue.pop(false));

	CPPUNIT_ASSERT(queue.pop(false).isNull());
}

/**
 * Schedule 3 works where one of them is suspended.
 */
void BasicQueueTest::testScheduleWithSuspend()
{
	Timestamp now;
	BasicQueue queue;

	CPPUNIT_ASSERT(queue.pop(false).isNull());

	Work::Ptr work0(new Work(WorkID::parse("7a06afdf-7733-4722-892f-bef2019fafed")));
	work0->setCreated(now - 15);
	work0->setSleepDuration(10);

	Work::Ptr work1(new Work(WorkID::random()));
	work1->setCreated(now - 15);
	work1->setSleepDuration(4);

	// This work must be scheduled between work1 and work 0.
	Work::Ptr work2(new Work(WorkID::random()));
	work2->setCreated(now - 25);
	work2->setSleepDuration(6);
	work2->setSuspended(now - 15);

	do {
		FastMutex::ScopedLock guard(queue.lock());

		queue.pushUnlocked(work0);
		queue.pushUnlocked(work1);
		queue.pushUnlocked(work2);
	} while (0);

	// remove and test order
	CPPUNIT_ASSERT(work1 == queue.pop(false));
	CPPUNIT_ASSERT(work2 == queue.pop(false));
	CPPUNIT_ASSERT(work0 == queue.pop(false));

	CPPUNIT_ASSERT(queue.pop(false).isNull());
}

/**
 * Work in state FINISHED or FAILED must not be scheduled.
 */
void BasicQueueTest::testScheduleIsNotDone()
{
	BasicQueue queue;

	CPPUNIT_ASSERT(queue.pop(false).isNull());

	Work::Ptr work(new Work(WorkID::parse("86adc330-0da5-4930-b992-53a5579e1470")));

	work->setState(Work::STATE_FINISHED);

	do {
		FastMutex::ScopedLock guard(queue.lock());
		queue.pushUnlocked(work);
	} while (0);

	CPPUNIT_ASSERT(queue.pop(false).isNull());

	work->setState(Work::STATE_FAILED);

	do {
		FastMutex::ScopedLock guard(queue.lock());
		queue.pushUnlocked(work);
	} while (0);

	CPPUNIT_ASSERT(queue.pop(false).isNull());
}

/**
 * Test we can cancel works out-of order.
 */
void BasicQueueTest::testCancelOutOfOrder()
{
	BasicQueue queue;

	CPPUNIT_ASSERT(queue.pop(false).isNull());

	Work::Ptr work0(new Work(WorkID::parse("f5ba244d-758b-419f-a7c9-d1e525510154")));
	work0->setSleepDuration(0);

	Work::Ptr work1(new Work(WorkID::parse("3bbfbf24-0207-40f9-9742-5089265e25a2")));
	work1->setSleepDuration(0);

	Work::Ptr work2(new Work(WorkID::parse("c5f08deb-a694-4fb0-8843-438a62fefe0e")));
	work2->setSleepDuration(0);

	do {
		FastMutex::ScopedLock guard(queue.lock());

		queue.pushUnlocked(work0);
		queue.pushUnlocked(work1);
		queue.pushUnlocked(work2);
	} while (0);

	do {
		FastMutex::ScopedLock guard(queue.lock());
		queue.cancelUnlocked(work1);
	} while (0);

	CPPUNIT_ASSERT(queue.pop(false) == work0);
	CPPUNIT_ASSERT(queue.pop(false) == work2);
	CPPUNIT_ASSERT(queue.pop(false).isNull());
}


/**
 * Make sure that remove of unexisting work does not
 * make any bad.
 */
void BasicQueueTest::testCancelNonExisting()
{
	BasicQueue queue;
	Work::Ptr work(new Work);

	FastMutex::ScopedLock guard(queue.lock());
	queue.cancelUnlocked(work);
}

/**
 * Test the queue does not pop any work that is planned
 * into the future. In non-blocking mode, we should
 * get no work.
 */
void BasicQueueTest::testCurrentlyNothing()
{
	Timestamp now;
	BasicQueue queue;

	CPPUNIT_ASSERT(queue.pop(false).isNull());

	Work::Ptr work(new Work(WorkID::parse("b19e70de-25b7-466e-b384-35f59e00c41d")));
	work->setCreated(now);
	// quite far future
	work->setSleepDuration(Timespan(60 * 60 * 24, 0));

	do {
		FastMutex::ScopedLock guard(queue.lock());
		queue.pushUnlocked(work);
	} while (0);

	CPPUNIT_ASSERT(queue.pop(false).isNull());
}

/**
 * Test that blocking pop() works for simple case.
 */
void BasicQueueTest::testSimpleBlocking()
{
	Timestamp now;
	BasicQueue queue;

	CPPUNIT_ASSERT(queue.pop(false).isNull());

	Work::Ptr work(new Work(WorkID::parse("5254bd68-7857-4c4e-b4ff-b6a7eb7e7168")));
	work->setCreated(now);
	// execute in about 100 ms
	work->setSleepDuration(Timespan(0, 100000));

	do {
		FastMutex::ScopedLock guard(queue.lock());
		queue.pushUnlocked(work);
	} while (0);

	CPPUNIT_ASSERT(Timestamp() - now < 100000);
	CPPUNIT_ASSERT(queue.pop(true) == work);
	CPPUNIT_ASSERT(Timestamp() - now >= 100000);
	CPPUNIT_ASSERT(queue.pop(false).isNull());
}

class DoLater {
public:
	DoLater(BasicQueue &queue, Work::Ptr work):
		m_queue(queue),
		m_work(work),
		m_callback(*this, &DoLater::fire)
	{
	}

	void start(Timer &timer)
	{
		timer.start(m_callback);
	}

protected:
	void fire(Timer &)
	{
		fireImpl();
	}

	virtual void fireImpl() = 0;

protected:
	BasicQueue &m_queue;
	Work::Ptr m_work;

private:
	TimerCallback<DoLater> m_callback;
};

class ScheduleLater : public DoLater {
public:
	ScheduleLater(BasicQueue &queue, Work::Ptr work):
		DoLater(queue, work)
	{
	}

	void fireImpl() override
	{
		do {
			FastMutex::ScopedLock guard(m_queue.lock());
			m_queue.pushUnlocked(m_work);
		} while (0);
	}
};

/**
 * Test blocking pop() while scheduling other works asynchronously in other
 * threads. The test contains race conditions that might influence the results
 * (but should not as the timing is quite relaxed). If all the tested Works are
 * scheduled on time (this is a precondition of the test), the order in which they
 * are pop'd out must be preserved.
 */
void BasicQueueTest::testBlockingWithAsyncSchedules()
{
	Timestamp now;
	BasicQueue queue;

	CPPUNIT_ASSERT(queue.pop(false).isNull());

	Work::Ptr work0(new Work(WorkID::parse("429273e5-1e98-4b90-a54c-207d22aeaff8")));
	work0->setCreated(now);
	// execute in about 100 ms
	work0->setSleepDuration(Timespan(0, 100000));

	Work::Ptr work1(new Work(WorkID::parse("76411862-df5e-4aef-a1bf-15530028e706")));
	work1->setCreated(now);
	// execute in about 40 ms
	work1->setSleepDuration(Timespan(0, 40000));

	Work::Ptr work2(new Work(WorkID::parse("0a267d6d-2eb1-49f6-99c2-ab15b993586e")));
	work2->setCreated(now);
	// execute in about 120 ms
	work2->setSleepDuration(Timespan(0, 120000));

	// defer scheduling of work1 after 20 ms when already
	// blocking in the pop() method
	Timer deferAfter20(21, 0);
	ScheduleLater scheduleWork1(queue, work1);

	// defer scheduling of work2 after 60 ms when blocking
	// in the pop() method
	Timer deferAfter60(61, 0);
	ScheduleLater scheduleWork2(queue, work2);

	scheduleWork1.start(deferAfter20);
	scheduleWork2.start(deferAfter60);

	do {
		FastMutex::ScopedLock guard(queue.lock());
		queue.pushUnlocked(work0);
	} while (0);

	CPPUNIT_ASSERT(Timestamp() - now < 40000);
	CPPUNIT_ASSERT(queue.pop(true) == work1);
	CPPUNIT_ASSERT(Timestamp() - now >= 40000);

	CPPUNIT_ASSERT(Timestamp() - now < 100000);
	CPPUNIT_ASSERT(queue.pop(true) == work0);
	CPPUNIT_ASSERT(Timestamp() - now >= 100000);

	CPPUNIT_ASSERT(Timestamp() - now < 120000);
	CPPUNIT_ASSERT(queue.pop(true) == work2);
	CPPUNIT_ASSERT(Timestamp() - now >= 120000);

	CPPUNIT_ASSERT(queue.pop(false).isNull());
}

class WakeupLater : public DoLater {
public:
	WakeupLater(BasicQueue &queue, Work::Ptr work):
		DoLater(queue, work)
	{
	}

	void fireImpl() override
	{
		do {
			FastMutex::ScopedLock guard(m_queue.lock());
			m_queue.wakeupUnlocked(m_work);
		} while (0);
	}
};

/**
 * Schedule work and pop(blocking) for it. In a separate
 * thread wakeup the work much earlier.
 */
void BasicQueueTest::testBlockingWithWakeup()
{
	Timestamp now;
	BasicQueue queue;

	CPPUNIT_ASSERT(queue.pop(false).isNull());

	Work::Ptr work(new Work(WorkID::parse("0595348e-1770-4914-a572-c29925edf73e")));
	work->setCreated(now);
	// execute in about 100 ms
	work->setSleepDuration(Timespan(0, 100000));

	// defer wakeup of work after 30 ms
	Timer deferAfter30(31, 0);
	WakeupLater wakeupWork(queue, work);

	do {
		FastMutex::ScopedLock guard(queue.lock());
		queue.pushUnlocked(work);
	} while (0);

	wakeupWork.start(deferAfter30);

	CPPUNIT_ASSERT(Timestamp() - now < 30000);
	CPPUNIT_ASSERT(queue.pop(true) == work);
	CPPUNIT_ASSERT(Timestamp() - now >= 30000);
	CPPUNIT_ASSERT(Timestamp() - now < 100000);

	CPPUNIT_ASSERT(queue.pop(false).isNull());
}

/**
 * Works work0 and work1 wait for event and work2
 * waits for both event or timeout. Force wakeup
 * work0 and work2 and remove them immediately.
 * The work1 stays in the queue suspended.
 */
void BasicQueueTest::testWakeupWaiting()
{
	Timestamp now;
	BasicQueue queue;

	CPPUNIT_ASSERT(queue.pop(false).isNull());

	Work::Ptr work0(new Work(WorkID::parse("21b4d091-e58b-4207-8b53-3488bd62218a")));
	work0->setNoSleepDuration();
	Work::Ptr work1(new Work(WorkID::parse("cfd17e3d-f04e-45c1-a2ea-adc9d1c10b8f")));
	work1->setNoSleepDuration();

	Work::Ptr work2(new Work(WorkID::parse("c452d7c6-112d-42fa-a1b5-1a3ae399dd1c")));
	work2->setCreated(now);
	// execute in about 100 ms or on wakeup
	work2->setSleepDuration(Timespan(0, 100000));

	// defer wakeup of work0 after 30 ms
	Timer deferAfter30(31, 0);
	WakeupLater wakeupWork0(queue, work0);

	// defer wakeup of work2 after 60 ms
	Timer deferAfter60(61, 0);
	WakeupLater wakeupWork2(queue, work2);

	do {
		FastMutex::ScopedLock guard(queue.lock());

		queue.pushUnlocked(work0);
		queue.pushUnlocked(work1);
		queue.pushUnlocked(work2);
	} while (0);

	wakeupWork0.start(deferAfter30);
	wakeupWork2.start(deferAfter60);

	CPPUNIT_ASSERT(Timestamp() - now < 30000);
	CPPUNIT_ASSERT(queue.pop(true) == work0);
	CPPUNIT_ASSERT(Timestamp() - now >= 30000);

	CPPUNIT_ASSERT(Timestamp() - now < 60000);
	CPPUNIT_ASSERT(queue.pop(true) == work2);
	CPPUNIT_ASSERT(Timestamp() - now >= 60000);

	CPPUNIT_ASSERT(queue.pop(false).isNull());
}

/**
 * Make sure that wakeup of unexisting work does not
 * make any bad.
 */
void BasicQueueTest::testWakeupNonexisting()
{
	BasicQueue queue;
	Work::Ptr work(new Work);

	FastMutex::ScopedLock guard(queue.lock());
	queue.wakeupUnlocked(work);
}


}
