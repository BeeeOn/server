#include <cmath>

#include <Poco/Exception.h>

#include "di/Injectable.h"
#include "service/ControlServiceImpl.h"
#include "util/Exception.h"

BEEEON_OBJECT_BEGIN(BeeeOn, ControlServiceImpl)
BEEEON_OBJECT_CASTABLE(ControlService)
BEEEON_OBJECT_PROPERTY("controlDao", &ControlServiceImpl::setControlDao)
BEEEON_OBJECT_PROPERTY("deviceDao", &ControlServiceImpl::setDeviceDao)
BEEEON_OBJECT_PROPERTY("gatewayRPC", &ControlServiceImpl::setGatewayRPC)
BEEEON_OBJECT_PROPERTY("accessPolicy", &ControlServiceImpl::setAccessPolicy)
BEEEON_OBJECT_PROPERTY("transactionManager", &ControlServiceImpl::setTransactionManager)
BEEEON_OBJECT_HOOK("done", &ControlServiceImpl::fixupControls)
BEEEON_OBJECT_END(BeeeOn, ControlServiceImpl)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

static const Timespan REQUEST_TIMEOUT_MIN = 1 * Timespan::SECONDS;
static const Timespan REQUEST_TIMEOUT_MAX = 2 * Timespan::MINUTES;

void ControlServiceImpl::setDeviceDao(DeviceDao::Ptr dao)
{
	m_deviceDao = dao;
}

void ControlServiceImpl::setControlDao(ControlDao::Ptr dao)
{
	m_controlDao = dao;
}

void ControlServiceImpl::setGatewayRPC(GatewayRPC::Ptr rpc)
{
	m_gatewayRPC = rpc;
}

void ControlServiceImpl::setAccessPolicy(ControlAccessPolicy::Ptr policy)
{
	m_accessPolicy = policy;
}

bool ControlServiceImpl::doFetch(Relation<Control, Device> &data)
{
	Control &control = data.target();
	Device device = data.base();

	m_accessPolicy->assure(
		ControlAccessPolicy::ACTION_USER_GET,
		data, control, device);

	if (!m_deviceDao->fetch(device, device.gateway()))
		throw NotFoundException("device " + device);

	return m_controlDao->fetch(data.target(), device);
}

void ControlServiceImpl::doFetchMany(Relation<std::list<Control>, Device> &data)
{
	Device device = data.base();

	m_accessPolicy->assureMany(ControlAccessPolicy::ACTION_USER_GET, data, device);

	if (!m_deviceDao->fetch(device, device.gateway()))
		throw NotFoundException("device " + device);

	std::list<Control> result;
	m_controlDao->fetchBy(result, device);

	for (const auto &control : result) {
		try {
			m_accessPolicy->assure(ControlAccessPolicy::ACTION_USER_GET,
					data, control, device);
		} catch (const Exception &e) {
			if (logger().debug()) // only while debugging
				logger().log(e, __FILE__, __LINE__);

			continue;
		}

		data.target().emplace_back(control);
	}
}

void ControlServiceImpl::fixupControls()
{
	size_t count = BEEEON_TRANSACTION_RETURN(size_t,
				m_controlDao->cancelUnfinished());

	if (count > 0) {
		logger().warning("cancelled "
				+ to_string(count)
				+ " control requests",
			__FILE__, __LINE__);
	}
}

void ControlServiceImpl::startRequest(
		const UserPolicyContext &policyContext,
		const Device &device,
		Control &control,
		double value)
{
	const Timestamp now;

	Control::RequestedValue request(policyContext.user(), now, value);
	control.setRequestedValue(request);

	if (logger().debug()) {
		logger().debug(
			"requesting change of control " + control,
			__FILE__, __LINE__);
	}

	m_controlDao->insert(request, control, device);
}

void ControlServiceImpl::cancelRequest(
		const Device &device,
		Control &control)
{
	const Timestamp now;

	Control::RequestedValue oldRequest(control.requestedValue());
	oldRequest.setResult(Control::RequestedValue::RESULT_FAILURE);
	oldRequest.setFinishedAt(now);

	if (logger().debug()) {
		logger().debug(
			"cancel current request and start another"
			" for control "
			+ control,
			__FILE__, __LINE__);
	}

	m_controlDao->markFinished(oldRequest, control, device);
}

ControlChangeHandler::Ptr ControlServiceImpl::doRequestChange(
		Relation<Control, Device> &data,
		double value,
		const Poco::Timespan &timeout,
		bool force)
{
	if (timeout < REQUEST_TIMEOUT_MIN)
		throw InvalidArgumentException("too short timeout for control change");
	if (timeout > REQUEST_TIMEOUT_MAX)
		throw InvalidArgumentException("too long timeout for control change");

	Device device = data.base();
	Control &control = data.target();

	m_accessPolicy->assure(
		ControlAccessPolicy::ACTION_USER_SET,
		data, control, device);

	if (std::isnan(value)) {
		throw InvalidArgumentException(
			"requesting to set control "
			+ device + ":" + control + " to NaN");
	}

	if (!m_deviceDao->fetch(device, device.gateway()))
		throw NotFoundException("device " + device);

	if (!device.status().active())
		throw NotFoundException("device " + device + " is inactive");

	if (!m_controlDao->fetch(control, device))
		throw NotFoundException("control " + control);

	const double recentValue = control.recentValue().value();
	const auto &requestedValue = control.requestedValue();

	if (recentValue == value) {
		if (!force) {
			logger().information("ignoring attempt to set the last reported value"
				+ to_string(recentValue) + " for control " + control,
				__FILE__, __LINE__);
			return nullptr;
		}
		else {
			logger().debug("force set the last reported value"
				+ to_string(recentValue) + " for control " + control,
				__FILE__, __LINE__);
		}
	}

	if (requestedValue.isActive()) {
		if (!force) {
			throw InProgressException(
				"set-value is already in progress for control "
				+ control);
		}
		else {
			logger().information(
				"cancelling current request for " + control,
				__FILE__, __LINE__);

			cancelRequest(device, control);
		}
	}

	startRequest(data, device, control, value);

	ControlChangeHandler::Ptr handler = new ControlChangeHandler(
			control, device, m_controlDao);
	handler->setTransactionManager(transactionManager());

	return handler;
}
