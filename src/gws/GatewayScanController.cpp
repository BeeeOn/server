#include <Poco/Clock.h>
#include <Poco/Exception.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/Logger.h>

#include "di/Injectable.h"
#include "gws/GatewayScanController.h"
#include "util/LambdaTimerTask.h"

BEEEON_OBJECT_BEGIN(BeeeOn, GatewayScanController)
BEEEON_OBJECT_CASTABLE(StoppableLoop)
BEEEON_OBJECT_PROPERTY("gatewayRPC", &GatewayScanController::setGatewayRPC)
BEEEON_OBJECT_PROPERTY("defaultDuration", &GatewayScanController::setDefaultDuration)
BEEEON_OBJECT_PROPERTY("cleanupInterval", &GatewayScanController::setCleanupInterval)
BEEEON_OBJECT_PROPERTY("cleanupOlderThan", &GatewayScanController::setCleanupOlderThan)
BEEEON_OBJECT_END(BeeeOn, GatewayScanController)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

const static Timespan MAX_SCAN_DURATION = 10 * Timespan::MINUTES;

GatewayScanController::ScanHandler::ScanHandler(const GatewayID &id):
	m_id(id)
{
}

const GatewayScan &GatewayScanController::ScanHandler::scan() const
{
	ScopedLock guard(const_cast<ScanHandler&>(*this));
	return m_scan;
}

GatewayScan &GatewayScanController::ScanHandler::scan()
{
	ScopedLock guard(const_cast<ScanHandler&>(*this));
	return m_scan;
}

void GatewayScanController::ScanHandler::onPending(GatewayRPCResult::Ptr r)
{
	ScanHandler::ScopedLock guard(*this);

	Logger &logger = Loggable::forClass(typeid(GatewayScanController));
	logger.information("scanning " + m_id.toString() + " requested",
			__FILE__, __LINE__);
	m_scan.changeState(GatewayScan::SCAN_PROCESSING);
}

void GatewayScanController::ScanHandler::onAccepted(GatewayRPCResult::Ptr r)
{
	ScanHandler::ScopedLock guard(*this);

	Logger &logger = Loggable::forClass(typeid(GatewayScanController));
	logger.information("scanning " + m_id.toString() + " is starting",
			__FILE__, __LINE__);
	m_scan.changeState(GatewayScan::SCAN_PROCESSING);
}

void GatewayScanController::ScanHandler::onSuccess(GatewayRPCResult::Ptr r)
{
	ScanHandler::ScopedLock guard(*this);

	Logger &logger = Loggable::forClass(typeid(GatewayScanController));
	logger.information("scanning " + m_id.toString() + " in progress",
			__FILE__, __LINE__);
	m_scan.changeState(GatewayScan::SCAN_PROCESSING);
}

void GatewayScanController::ScanHandler::onFailed(GatewayRPCResult::Ptr r)
{
	ScanHandler::ScopedLock guard(*this);

	Logger &logger = Loggable::forClass(typeid(GatewayScanController));
	logger.error("scanning " + m_id.toString() + " has failed",
			__FILE__, __LINE__);

	m_scan.changeState(GatewayScan::SCAN_FAILED);
}

void GatewayScanController::ScanHandler::onTimeout(GatewayRPCResult::Ptr r)
{
	ScanHandler::ScopedLock guard(*this);

	Logger &logger = Loggable::forClass(typeid(GatewayScanController));
	logger.error("scanning " + m_id.toString() + " did not start on time",
			__FILE__, __LINE__);

	m_scan.changeState(GatewayScan::SCAN_FAILED);
}

void GatewayScanController::ScanHandler::onNotConnected(GatewayRPCResult::Ptr r)
{
	ScanHandler::ScopedLock guard(*this);

	Logger &logger = Loggable::forClass(typeid(GatewayScanController));
	logger.error("scanning " + m_id.toString() + " failed, not connected",
			__FILE__, __LINE__);

	m_scan.changeState(GatewayScan::SCAN_FAILED);
}

GatewayScanController::CleanUpTask::CleanUpTask(
		FastMutex &lock,
		GatewayScanController::ScanMap &scanMap):
	m_olderThen(30 * Timespan::MINUTES),
	m_lock(lock),
	m_scanMap(scanMap)
{
}

void GatewayScanController::CleanUpTask::setOlderThan(
		const Poco::Timespan &olderThen)
{
	if (olderThen < 0)
		throw InvalidArgumentException("cleanup older-than timeout must be positive");

	m_olderThen = olderThen;
}

void GatewayScanController::CleanUpTask::run()
{
	FastMutex::ScopedLock guard(m_lock);
	const Clock started;
	const size_t total = m_scanMap.size();
	size_t count = 0;

	logger().information("cleaning up scan records ("
			+ to_string(total) + ")",
			__FILE__, __LINE__);

	for (auto it = m_scanMap.begin(); it != m_scanMap.end();) {
		ScanHandler::Ptr context = it->second;
		ScanHandler::ScopedLock guard(*context);

		if (logger().debug()) {
			logger().debug("cleaning up scan record for "
					+ it->first.toString(),
					__FILE__, __LINE__);
		}

		const auto &started = context->scan().started();
		const auto &duration = context->scan().duration();
		const Timestamp now;

		switch (context->scan().state()) {
		case GatewayScan::SCAN_IDLE:
			it = m_scanMap.erase(it);
			count += 1;
			break;

		case GatewayScan::SCAN_FINISHED:
		case GatewayScan::SCAN_FAILED:
			if (started + duration < now - m_olderThen) {
				it = m_scanMap.erase(it);
				count += 1;
			}
			else {
				++it;
			}

			break;

		case GatewayScan::SCAN_WAITING:
		case GatewayScan::SCAN_PROCESSING:
			if (started + duration < now) {
				logger().warning("scan record for " + it->first.toString()
						+ " was not stopped on time, marking as failed",
						__FILE__, __LINE__);
				context->scan().changeState(GatewayScan::SCAN_FAILED);
			}

			++it;
			break;

		default:
			++it;
		}
	}

	logger().notice("cleaned up "
			+ to_string(count) + "/" + to_string(total)
			+ " scan records in "
			+ to_string(started.elapsed()) + "us",
			__FILE__, __LINE__);
}

GatewayScanController::GatewayScanController():
	m_defaultDuration(2 * Timespan::MINUTES),
	m_cleanupInterval(15 * Timespan::MINUTES),
	m_cleanupTask(new CleanUpTask(m_lock, m_scanMap))
{
}

void GatewayScanController::setDefaultDuration(const Poco::Timespan &duration)
{
	if (duration.totalSeconds() <= 0)
		throw InvalidArgumentException("duration must be at least a second");

	if (duration > MAX_SCAN_DURATION)
		throw InvalidArgumentException("too long default duration");

	m_defaultDuration = duration;
}

void GatewayScanController::setCleanupInterval(const Timespan &interval)
{
	if (interval.totalMilliseconds() <= 0)
		throw InvalidArgumentException("cleanup interval must be at least a millisecond");

	m_cleanupInterval = interval;
}

void GatewayScanController::setCleanupOlderThan(const Timespan &time)
{
	m_cleanupTask->setOlderThan(time);
}

void GatewayScanController::setGatewayRPC(GatewayRPC::Ptr rpc)
{
	m_rpc = rpc;
}

void GatewayScanController::start()
{
	logger().information("starting gateway scan controller");

	m_timer.scheduleAtFixedRate(
		m_cleanupTask,
		m_cleanupInterval.totalMilliseconds(),
		m_cleanupInterval.totalMilliseconds());
}

void GatewayScanController::stop()
{
	logger().information("stopping gateway scan controller");

	m_timer.cancel(true);

	FastMutex::ScopedLock guard(m_lock);
	m_scanMap.clear();
}

GatewayScan GatewayScanController::scan(const Gateway &gateway, const Timespan &timeout)
{
	const Timespan &duration = timeout <= 0? m_defaultDuration : timeout;

	if (duration > MAX_SCAN_DURATION)
		throw InvalidArgumentException("too big scanning timeout given");

	const auto id = gateway.id();

	ScopedLockWithUnlock<FastMutex> guard(m_lock);

	auto it = m_scanMap.find(id);
	if (it != m_scanMap.end())
		return it->second->scan();

	ScanHandler::Ptr context = new ScanHandler(id);
	context->scan().setDuration(duration);

	m_scanMap.emplace(id, context);

	guard.unlock();

	const Clock start;

	m_rpc->sendListen(
		context,
		gateway,
		duration
	);

	m_timer.schedule(new LambdaTimerTask([start, id, context]() mutable
		{
			Logger &logger = Loggable::forClass(typeid(GatewayScanController));

			ScanHandler::ScopedLock guard(*context);
			GatewayScan &scan = context->scan();

			const Timespan &duration = scan.started().elapsed();

			switch (scan.state()) {
			case GatewayScan::SCAN_FINISHED:
				logger.warning("scanning of "
						+ id.toString()
						+ " is already finished",
						__FILE__, __LINE__);
				break;

			case GatewayScan::SCAN_WAITING:
			case GatewayScan::SCAN_PROCESSING:
				logger.information("scanning of "
						+ id.toString()
						+ " was finished after "
						+ DateTimeFormatter::format(duration),
						__FILE__, __LINE__);

				scan.changeState(GatewayScan::SCAN_FINISHED);
				break;

			default:
				if (logger.debug()) {
					logger.debug("skipping explicit finish of "
							+ id.toString() + " scan",
							__FILE__, __LINE__);
				}
				break;
			}
		}),
		start + duration.totalMicroseconds());

	return context->scan();
}

GatewayScan GatewayScanController::find(const Gateway &gateway)
{
	FastMutex::ScopedLock guard(m_lock);

	auto it = m_scanMap.find(gateway.id());
	if (it != m_scanMap.end())
		return it->second->scan();

	return GatewayScan::createIdle();
}
