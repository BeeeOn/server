#include <Poco/Exception.h>
#include <Poco/Thread.h>
#include <Poco/Logger.h>

#include "loop/StoppableLoop.h"

using namespace Poco;
using namespace BeeeOn;

StoppableLoop::~StoppableLoop()
{
}

StoppableLoopAdapter::StoppableLoopAdapter(
		SharedPtr<StoppableRunnable> runnable):
	m_stopTimeout(10000),
	m_runnable(runnable),
	m_thread(NULL)
{
	if (m_runnable.isNull())
		throw InvalidArgumentException("runnable must not be NULL");
}

StoppableLoopAdapter::~StoppableLoopAdapter()
{
	doStop();
}

void StoppableLoopAdapter::setStopTimeout(long ms)
{
	if (ms <= 0)
		throw InvalidArgumentException("stopTimeout must be greater then 0");

	m_stopTimeout = ms;
}

void StoppableLoopAdapter::start()
{
	if (m_thread != NULL && m_thread->isRunning())
		return;

	if (m_thread != NULL && !m_thread->isRunning())
		delete m_thread;

	m_thread = new Thread();
	m_thread->start(*m_runnable);
}

void StoppableLoopAdapter::stop()
{
	doStop();
}

void StoppableLoopAdapter::doStop()
{
	if (m_thread == NULL || !m_thread->isRunning())
		return;

	m_runnable->stop();

	try {
		m_thread->join(m_stopTimeout);
		delete m_thread;
	} catch (const TimeoutException &e) {
		/*
		 Do not delete the thread, there is no
		 portable way to stop it. If it contains
		 a bug, leave it as it is and just log
		 this fact.
		 */
		logger().log(e, __FILE__, __LINE__);
	}

	m_thread = NULL;
}
