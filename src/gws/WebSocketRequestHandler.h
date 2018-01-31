#ifndef BEEEON_WEBSOCKET_REQUEST_HANDLER_H
#define BEEEON_WEBSOCKET_REQUEST_HANDLER_H

#include <string>

#include <Poco/Net/AbstractHTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/WebSocket.h>

#include "gws/GatewayCommunicator.h"
#include "gws/SocketGatewayPeerVerifierFactory.h"
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
	public Poco::Net::AbstractHTTPRequestHandler,
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
	void run();

protected:
	/**
	 * Process the received WebSocket frame, generate a response and
	 * register the gateway with GatewayCommunicator.
	 */
	void processPayload(
		Poco::Net::WebSocket &ws,
		std::string data);

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
			GWSGatewayService::Ptr service,
			SocketGatewayPeerVerifierFactory::Ptr verifierFactory):
		m_maxMessageSize(maxMessageSize),
		m_gatewayCommunicator(communicator),
		m_gatewayService(service),
		m_verifierFactory(verifierFactory)
	{
	}

	Poco::Net::HTTPRequestHandler *createRequestHandler(
		const Poco::Net::HTTPServerRequest &request) override;

private:
	size_t m_maxMessageSize;
	GatewayCommunicator::Ptr m_gatewayCommunicator;
	GWSGatewayService::Ptr m_gatewayService;
	SocketGatewayPeerVerifierFactory::Ptr m_verifierFactory;
};

}

#endif
