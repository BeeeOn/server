#include <Poco/Exception.h>
#include <Poco/Thread.h>
#include <Poco/Logger.h>

#include "di/Injectable.h"
#include "work/BasicProcessor.h"
#include "work/WorkBackup.h"
#include "work/WorkExecutor.h"
#include "work/WorkRunner.h"
#include "work/WorkScheduler.h"

BEEEON_OBJECT_BEGIN(BeeeOn, BasicProcessor)
BEEEON_OBJECT_CASTABLE(WorkScheduler)
BEEEON_OBJECT_CASTABLE(StoppableRunnable)
BEEEON_OBJECT_REF("backup", &BasicProcessor::setBackup)
BEEEON_OBJECT_REF("runnerFactory", &BasicProcessor::setRunnerFactory)
BEEEON_OBJECT_REF("lockManager", &BasicProcessor::setLockManager)
BEEEON_OBJECT_REF("executors", &BasicProcessor::registerExecutor)
BEEEON_OBJECT_NUMBER("minThreads", &BasicProcessor::setMinThreads)
BEEEON_OBJECT_NUMBER("maxThreads", &BasicProcessor::setMaxThreads)
BEEEON_OBJECT_NUMBER("threadIdleTime", &BasicProcessor::setThreadIdleTime)
BEEEON_OBJECT_HOOK("done", &BasicProcessor::init)
BEEEON_OBJECT_END(BeeeOn, BasicProcessor)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

BasicProcessor::BasicProcessor():
	m_backup(&EmptyWorkBackup::instance()),
	m_runnerFactory(&NullWorkRunnerFactory::instance()),
	m_shouldStop(0),
	m_current(NULL),
	m_minThreads(1),
	m_maxThreads(16),
	m_threadIdleTime(100)
{
}

void BasicProcessor::setBackup(WorkBackup *backup)
{
	m_backup = backup? backup : &EmptyWorkBackup::instance();
}

void BasicProcessor::setRunnerFactory(WorkRunnerFactory *factory)
{
	m_runnerFactory = factory? factory : &NullWorkRunnerFactory::instance();
}

void BasicProcessor::setLockManager(WorkLockManager::Ptr manager)
{
	m_lockManager = manager;
}

void BasicProcessor::setMinThreads(int min)
{
	if (min < 0)
		throw InvalidArgumentException("minThreads must be non-negative");

	m_minThreads = min;
}

void BasicProcessor::setMaxThreads(int max)
{
	if (max < 0)
		throw InvalidArgumentException("maxThreads must be non-negative");

	m_maxThreads = max;
}

void BasicProcessor::setThreadIdleTime(int ms)
{
	if (ms <= 0)
		throw InvalidArgumentException("threadIdleTime must be greater then zero");

	if (ms > 0 && ms < 1000) {
		logger().warning("threadIdleTime's granularity is 1000 ms, treating "
				+ to_string(ms) + " as zero",
				__FILE__, __LINE__);
	}

	m_threadIdleTime = ms / 1000;
}

void BasicProcessor::registerExecutor(WorkExecutor *executor)
{
	m_executors.push_back(executor);
}

void BasicProcessor::init()
{
	initQueue();
	initPool();
}

void BasicProcessor::initQueue()
{
	vector<Work::Ptr> all;
	m_backup->loadScheduled(all);

	FastMutex::ScopedLock guard(m_queue.lock());

	for (auto &work : all) {
		if (work->state() == Work::STATE_EXECUTED) {
			if (logger().debug()) {
				logger().debug("work " + *work
						+ " in state executed is marked as failed",
						__FILE__, __LINE__);
			}

			// locking is unnecessary as this is to be called only during
			// initialization when the BasicProcessor is inactive.

			work->setState(Work::STATE_FAILED);
			m_backup->store(work);
			continue;
		}
		else if (work->state() >= Work::STATE_FINISHED)
			continue;

		WorkWriteGuard guard(m_lockManager->readWrite(work->id()));
		m_queue.pushUnlocked(work);
	}
}

void BasicProcessor::initPool()
{
	if (m_pool.isNull()) {
		logger().notice("creating thread pool min: "
			+ to_string(m_minThreads)
			+ " max: "
			+ to_string(m_maxThreads),
			__FILE__, __LINE__);

		m_pool = new ThreadPool(
			m_minThreads,
			m_maxThreads,
			m_threadIdleTime
		);
	}
}

ThreadPool &BasicProcessor::pool()
{
	initPool();
	return *m_pool;
}

void BasicProcessor::run()
{
	if (m_current != NULL)
		return;

	m_current = Thread::current();

	if (m_current == NULL)
		throw IllegalStateException("called outside of thread",
				__FILE__, __LINE__);

	m_current->setName("BasicProcessor");

	while (!m_shouldStop) {
		try {
			dispatch();
		}
		catch (const Poco::Exception &e) {
			logger().log(e, __FILE__, __LINE__);
		}
		catch (const std::exception &e) {
			logger().critical(e.what(), __FILE__, __LINE__);
			break;
		}
		catch (...) {
			logger().critical("failed to dispatch", __FILE__, __LINE__);
			break;
		}
	}
}

void BasicProcessor::stop()
{
	m_shouldStop = 1;
	notify();
}

void BasicProcessor::dispatch()
{
	Work::Ptr work = m_queue.pop();
	if (work.isNull()) {
		if (logger().debug()) {
			logger().debug("no work in the queue",
					__FILE__, __LINE__);
		}

		return;
	}

	if (Work::timestampValid(work->finished())) {
		if (logger().debug()) {
			logger().debug("work " + *work
				+ " has finished timestamp, skipping",
				__FILE__, __LINE__);
		}
		return;
	}

	if (work->state() >= Work::STATE_FINISHED) {
		logger().warning("work " + *work + " is done, ignoring...",
				__FILE__, __LINE__);
		return;
	}

	WorkExecutor *executor = NULL;

	for (auto one : m_executors) {
		if (!one->accepts(work))
			continue;

		executor = one;
		break;
	}

	if (executor == NULL) {
		logger().warning("no executor for work " + *work,
				__FILE__, __LINE__);

		work->setState(Work::STATE_FAILED);
		m_backup->store(work);
		return;
	}

	if (logger().debug())
		logger().debug("executing work " + *work, __FILE__, __LINE__);

	execute(executor, work);
}

void BasicProcessor::execute(WorkExecutor *executor, Work::Ptr work)
{
	WorkRunner *runner(m_runnerFactory->create(*this));
	if (runner == NULL) {
		logger().critical("runner was created as NULL",
				__FILE__, __LINE__);
		return;
	}

	if (logger().debug()) {
		logger().debug("having runner, executing work " + *work + " in thread",
				__FILE__, __LINE__);
	}

	runner->setExecutor(executor);
	runner->setWork(work);
	runner->setBackup(m_backup);

	try {
		while (!executeInThread(*runner, "Work:" + *work)) {
			logger().warning("no thread to execute work " + *work);
			m_current->trySleep(100);
		}
	} catch (...) {
		// WorkRunner did not started otherwise it
		// would delete itself automatically
		delete runner;
		throw;
	}
}

bool BasicProcessor::executeInThread(Runnable &runnable, const string &name)
{
	if (logger().debug())
		logger().debug("executing thread " + name, __FILE__, __LINE__);

	try {
		pool().start(runnable, name);
	} catch (Poco::NoThreadAvailableException &e) {
		return false;
	}

	return true;
}

void BasicProcessor::schedule(Work::Ptr work)
{
	if (logger().debug()) {
		logger().debug("schedule work " + *work,
				__FILE__, __LINE__);
	}

	FastMutex::ScopedLock guard(m_queue.lock());
	WorkWriteGuard workGuard(m_lockManager->readWrite(work->id()));

	m_queue.pushUnlocked(work);
	m_backup->store(work);

	wakeUpSelf();
}

void BasicProcessor::wakeup(Work::Ptr work)
{
	if (logger().debug()) {
		logger().debug("wakeup work " + *work,
				__FILE__, __LINE__);
	}

	FastMutex::ScopedLock guard(m_queue.lock());
	WorkWriteGuard workGuard(m_lockManager->readWrite(work->id()));

	m_queue.wakeupUnlocked(work);
	m_backup->store(work);

	wakeUpSelf();
}

void BasicProcessor::cancel(Work::Ptr work)
{
	if (logger().debug()) {
		logger().debug("cancel work " + *work,
				__FILE__, __LINE__);
	}

	FastMutex::ScopedLock guard(m_queue.lock());

	// lock the work to cancel it safely
	WorkWriteGuard workGuard(m_lockManager->readWrite(work->id()));

	m_queue.cancelUnlocked(work);
	work->setState(Work::STATE_CANCELED);
	m_backup->store(work);

	wakeUpSelf();
}

void BasicProcessor::notify()
{
	if (logger().debug()) {
		logger().debug("notify work processor",
				__FILE__, __LINE__);
	}

	m_queue.notify();
	wakeUpSelf();
}

void BasicProcessor::wakeUpSelf()
{
	if(m_current)
		m_current->wakeUp();
}
