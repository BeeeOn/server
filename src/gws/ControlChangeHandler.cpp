#include <Poco/Logger.h>

#include "gws/ControlChangeHandler.h"

using namespace Poco;
using namespace BeeeOn;

ControlChangeHandler::ControlChangeHandler(
		const Control &control,
		const Device &device,
		ControlDao::Ptr dao):
	m_control(control),
	m_device(device),
	m_controlDao(dao),
	m_finished(false)
{
}

ControlChangeHandler::~ControlChangeHandler()
{
	if (m_finished)
		return;

	if (logger().debug()) {
		logger().debug("cancelling control request for " + m_control,
				__FILE__, __LINE__);
	}

	try {
		finishRequest(true);
	}
	BEEEON_CATCH_CHAIN(logger())
}

const Control &ControlChangeHandler::control() const
{
	return m_control;
}

const Device &ControlChangeHandler::device() const
{
	return m_device;
}

void ControlChangeHandler::onPending(GatewayRPCResult::Ptr result)
{
	logger().information(
		"control " + m_control + " change is pending...",
		__FILE__, __LINE__);
}

void ControlChangeHandler::onAccepted(GatewayRPCResult::Ptr result)
{
	logger().information(
		"control " + m_control + " is being processed by gateway",
		__FILE__, __LINE__);

	const Timestamp now;

	BEEEON_TRANSACTION(
		Control control(m_control);

		if (!m_controlDao->fetch(control, m_device))
			throw NotFoundException("no such control " + control);

		auto request = control.requestedValue();

		if (!request.acceptedAt().isNull()) {
			logger().warning("ignoring accepted event for control " + m_control,
					__FILE__, __LINE__);
			return;
		}

		request.setAcceptedAt(now);
		control.setRequestedValue(request);

		m_controlDao->update(request, control, m_device);
		m_control = control;
	);
}

void ControlChangeHandler::finishRequest(bool failed)
{
	const Timestamp now;

	BEEEON_TRANSACTION(
		Control control(m_control);

		if (!m_controlDao->fetch(control, m_device))
			throw NotFoundException("no such control " + control);

		auto request = control.requestedValue();

		if (!request.finishedAt().isNull()) {
			logger().warning("finishing request for control "
				+ control
				+ " but it is already finished, skipping");
			return;
		}

		// accepted could be missed
		if (!failed && request.acceptedAt().isNull())
			request.setAcceptedAt(now);

		request.setFinishedAt(now);

		if (failed)
			request.setResult(Control::RequestedValue::RESULT_FAILURE);
		else
			request.setResult(Control::RequestedValue::RESULT_SUCCESS);

		m_controlDao->update(request, control, m_device);
		m_control = control;
	);

	m_finished = true;
}

void ControlChangeHandler::onSuccess(GatewayRPCResult::Ptr result)
{
	logger().information(
		"control " + m_control + " successfully changed",
		__FILE__, __LINE__);

	finishRequest(false);
}

void ControlChangeHandler::onFailed(GatewayRPCResult::Ptr result)
{
	logger().warning(
		"control " + m_control + " failed to change",
		__FILE__, __LINE__);

	finishRequest(true);
}

void ControlChangeHandler::onNotConnected(GatewayRPCResult::Ptr result)
{
	logger().warning(
		"control " + m_control + " failed to change, gateway "
		+ m_device.gateway() + " not connected",
		__FILE__, __LINE__);

	finishRequest(true);
}

void ControlChangeHandler::onTimeout(GatewayRPCResult::Ptr result)
{
	logger().warning(
		"control " + m_control + " failed to change, server timeout occured",
		__FILE__, __LINE__);

	finishRequest(true);
}
