#include <Poco/Exception.h>
#include <Poco/Timestamp.h>
#include <Poco/Net/NetException.h>

#include "di/Injectable.h"
#include "work/GatewayScanWorkExecutor.h"

BEEEON_OBJECT_BEGIN(BeeeOn, GatewayScanWorkExecutor)
BEEEON_OBJECT_CASTABLE(WorkExecutor)
BEEEON_OBJECT_REF("gatewayRPC", &GatewayScanWorkExecutor::setGatewayRPC)
BEEEON_OBJECT_REF("scheduler", &GatewayScanWorkExecutor::setScheduler)
BEEEON_OBJECT_REF("lockManager", &GatewayScanWorkExecutor::setLockManager)
BEEEON_OBJECT_END(BeeeOn, GatewayScanWorkExecutor)

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

GatewayScanWorkExecutor::GatewayScanWorkExecutor()
{
}

void GatewayScanWorkExecutor::setGatewayRPC(GatewayRPC::Ptr rpc)
{
	m_rpc = rpc;
}

bool GatewayScanWorkExecutor::accepts(const Work::Ptr work) const
{
	return work->content().type().is<GatewayScanWork>();
}

void GatewayScanWorkExecutor::processResult(Work::Ptr work, GatewayScanWork &content)
{
	const Gateway gateway(content.gatewayID());
	const Timespan elapsed(content.started().elapsed());

	if (!content.hasResult()) {
		logger().warning("spurious wakeup?", __FILE__, __LINE__);
		return;
	}

	switch (content.result()) {
	case GatewayRPCResult::Status::NOT_CONNECTED:
		throw ConnectionAbortedException(
			"failed to scan on gateway " + gateway);

	case GatewayRPCResult::Status::TIMEOUT:
		throw TimeoutException(
			"failed to scan on gateway " + gateway);

	case GatewayRPCResult::Status::FAILED:
		throw RuntimeException(
			"failed to scan on gateway " + gateway);

	case GatewayRPCResult::Status::SUCCESS:
		if (elapsed >= content.duration())
			return;

		suspend(work, content.duration() - elapsed);
		break;

	default:
		if (elapsed >= content.duration()) {
			throw TimeoutException(
				"scan not confirmed after "
				+ to_string(elapsed.totalSeconds())
				+ " seconds");
		}

		suspend(work, content.duration() - elapsed);
		break;
	}
}

void GatewayScanWorkExecutor::execute(Work::Ptr work)
{
	GatewayScanWork content = work->contentAs<GatewayScanWork>();

	if (Work::timestampValid(work->suspended())) {
		processResult(work, content);
		return;
	}

	const Gateway gateway(content.gatewayID());

	WorkScheduler::Ptr scheduler = this->scheduler();
	WorkLockManager::Ptr lockManager = this->lockManager();

	content.setStarted(Timestamp());
	work->setContent(content);

	m_rpc->sendListen([=](GatewayRPCResult::Ptr result) mutable
		{
			{
				WorkWriteGuard accessGuard(lockManager->readWrite(work->id()));

				GatewayScanWork content = work->contentAs<GatewayScanWork>();
				content.setResult(result->status());
				work->setContent(content);
			}

			switch (result->status()) {
			case GatewayRPCResult::Status::PENDING:
			case GatewayRPCResult::Status::ACCEPTED:
				// TODO persist
				return; // no other action needed
			default:
				scheduler->wakeup(work);
				break;
			}
		},
		gateway,
		content.duration()
	);

	suspend(work, content.duration());
}
