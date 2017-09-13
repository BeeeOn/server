#ifndef BEEEON_WEBSOCKET_REQUEST_HANDLER_H
#define BEEEON_WEBSOCKET_REQUEST_HANDLER_H

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>

#include "gws/GatewayCommunicator.h"
#include "service/GWSGatewayServiceImpl.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * @brief Handler for incoming WebSocket connection from the Gateway.
 *
 * The WebSocket connection is here initialized, registered and then passed
 * to the GatewayCommunicator. After the connection is added to the communicator,
 * this handler is destroyed.
 */
class WebSocketRequestHandler :
	public Poco::Net::HTTPRequestHandler,
	protected Loggable {
public:
	WebSocketRequestHandler(
			size_t maxMessageSize,
			GatewayCommunicator::Ptr communicator,
			GWSGatewayService::Ptr service):
		m_maxMessageSize(maxMessageSize),
		m_gatewayCommunicator(communicator),
		m_gatewayService(service)
	{
	}

	/**
	 * @brief Handles incoming WebSocket connection from the Gateway.
	 *
	 * After the connection is created, the register message is expected.
	 * Successfully registered Gateway connection (using GatewayService)
	 * is added to the GatewayCommunicator.
	 */
	void handleRequest(
		Poco::Net::HTTPServerRequest &request,
		Poco::Net::HTTPServerResponse &response);

private:
	size_t m_maxMessageSize;
	GatewayCommunicator::Ptr m_gatewayCommunicator;
	GWSGatewayService::Ptr m_gatewayService;
};

/**
 * @brief Factory for WebSocketRequestHandler objects.
 */
class WebSocketRequestHandlerFactory :
	public Poco::Net::HTTPRequestHandlerFactory {
public:
	WebSocketRequestHandlerFactory(
			size_t maxMessageSize,
			GatewayCommunicator::Ptr communicator,
			GWSGatewayService::Ptr service):
		m_maxMessageSize(maxMessageSize),
		m_gatewayCommunicator(communicator),
		m_gatewayService(service)
	{
	}

	Poco::Net::HTTPRequestHandler *createRequestHandler(
		const Poco::Net::HTTPServerRequest &request)
	{
		return new WebSocketRequestHandler(
			m_maxMessageSize,
			m_gatewayCommunicator,
			m_gatewayService
		);
	}
private:
	size_t m_maxMessageSize;
	GatewayCommunicator::Ptr m_gatewayCommunicator;
	GWSGatewayService::Ptr m_gatewayService;
};

}

#endif
