#pragma once

#include <map>

#include <Poco/Mutex.h>
#include <Poco/SharedPtr.h>
#include <Poco/SynchronizedObject.h>
#include <Poco/Timespan.h>

#include <Poco/Util/Timer.h>

#include "loop/StoppableLoop.h"
#include "model/Gateway.h"
#include "model/GatewayScan.h"
#include "rpc/GatewayRPC.h"
#include "rpc/GatewayRPCHandler.h"
#include "transaction/Transactional.h"

namespace BeeeOn {

/**
 * @brief Implementation of the gateway scanning control logic.
 * The GatewayScanController manages a set of GatewayScan instances
 * to maintain the currently running or finished scans.
 *
 * Each GatewayScan instance is maintained until it reaches its finish
 * or failing state. Finished or failed scans are either restarted by
 * a user or garbage collected by the GatewayScanController itself after
 * a predefined time period.
 *
 * A gateway scan process is always executed for a certain period of time.
 * After that period, the process is finished. The GatewayScanController
 * manages timers to finish each scanning process after that period.
 */
class GatewayScanController : public Transactional, public StoppableLoop {
public:
	typedef Poco::SharedPtr<GatewayScanController> Ptr;

	GatewayScanController();

	/**
	 * Default duration to use when the scan is called with negative
	 * or 0 timeout.
	 */
	void setDefaultDuration(const Poco::Timespan &duration);

	/**
	 * Set the interval between the CleanUpTask executions.
	 */
	void setCleanupInterval(const Poco::Timespan &interval);

	/**
	 * @see CleanUpTask::setOlderThan()
	 */
	void setCleanupOlderThan(const Poco::Timespan &time);

	void setGatewayRPC(GatewayRPC::Ptr rpc);

	/**
	 * Start the background cleanup task.
	 */
	void start() override;

	/**
	 * Stop all tasks.
	 */
	void stop() override;

	/**
	 * Initialize a new GatewayScan if none is in progress. If there is a GatewayScan
	 * instance, it is reused. This means that there cannot be 2 running scans on a
	 * single gateway.
	 */
	GatewayScan scan(const Gateway &gateway, const Poco::Timespan &timeout);

	/**
	 * Find a GatewayScan for the given gateway. If there is none, a new idle one
	 * is returned.
	 */
	GatewayScan find(const Gateway &gateway);

private:
	/**
	 * @brief Wrapper around the GatewayScan that adds the synchronized
	 * object properties and dynamic allocation management via the SharedPtr.
	 */
	class ScanHandler : public Poco::SynchronizedObject, public GatewayRPCHandler {
	public:
		typedef Poco::AutoPtr<ScanHandler> Ptr;

		ScanHandler(const GatewayID &id);

		const GatewayScan &scan() const;
		GatewayScan &scan();

		void onPending(GatewayRPCResult::Ptr r) override;
		void onAccepted(GatewayRPCResult::Ptr r) override;
		void onSuccess(GatewayRPCResult::Ptr r) override;
		void onFailed(GatewayRPCResult::Ptr r) override;
		void onTimeout(GatewayRPCResult::Ptr r) override;
		void onNotConnected(GatewayRPCResult::Ptr r) override;

	private:
		GatewayID m_id;
		GatewayScan m_scan;
	};

	/**
	 * Mapping Gateway IDs to their particular GatewayScan records.
	 */
	typedef std::map<GatewayID, ScanHandler::Ptr> ScanMap;

	/**
	 * @brief CleanUpTask is responsible for cleaning up the obsolete
	 * GatewayScan instances. When started, it removes GatewayScans
	 * which are idle, or which have finished/failed (must be in this
	 * state for at least olderThen period).
	 *
	 * GatewayScan instances that are still waiting or processing and
	 * are seems to not finished on time (due some bug) are changed to
	 * the state failed. This allows to garbage collect them during the
	 * next start of the CleanUpTask.
	 */
	class CleanUpTask : public Poco::Util::TimerTask, Loggable {
	public:
		typedef Poco::AutoPtr<CleanUpTask> Ptr;

		CleanUpTask(Poco::FastMutex &lock, ScanMap &scanMap);
		void run() override;

		/**
		 * Set the time period we have to way for each GatewayScan
		 * before they can be deleted after finish/failure.
		 */
		void setOlderThan(const Poco::Timespan &olderThen);

	private:
		Poco::Timespan m_olderThen;
		Poco::FastMutex &m_lock;
		ScanMap &m_scanMap;
	};

private:
	Poco::FastMutex m_lock;
	ScanMap m_scanMap;
	GatewayRPC::Ptr m_rpc;
	Poco::Timespan m_defaultDuration;
	Poco::Timespan m_cleanupInterval;
	CleanUpTask::Ptr m_cleanupTask;
	Poco::Util::Timer m_timer;
};

}
