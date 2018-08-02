#ifndef BEEEON_WEBSOCKET_REQUEST_HANDLER_H
#define BEEEON_WEBSOCKET_REQUEST_HANDLER_H

#include <string>

#include <Poco/SharedPtr.h>
#include <Poco/Net/WebSocket.h>

#include "gws/GatewayCommunicator.h"
#include "gws/GatewayPeerVerifier.h"
#include "gws/SocketGatewayPeerVerifierFactory.h"
#include "service/GWSGatewayService.h"
#include "server/WebSocketRequestHandler.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * @brief Handler for incoming WebSocket connection from the Gateway.
 *
 * The WebSocket connection is here initialized, registered and then passed
 * to the GatewayCommunicator. After the connection is added to the communicator,
 * this handler is destroyed.
 */
class GWRequestHandler : public WebSocketRequestHandler {
public:
	GWRequestHandler(
			size_t maxMessageSize,
			GatewayCommunicator::Ptr communicator,
			GWSGatewayService::Ptr service,
			GatewayPeerVerifier::Ptr peerVerifier);

protected:
	/**
	 * @brief Handles incoming WebSocket connection from the Gateway.
	 *
	 * After the connection is created, the register message is expected.
	 * Successfully registered Gateway connection (using GatewayService)
	 * is added to the GatewayCommunicator.
	 */
	void handle(Poco::Net::WebSocket &ws) override;

	/**
	 * Process the received WebSocket frame, generate a response and
	 * register the gateway with GatewayCommunicator.
	 */
	void processPayload(
		Poco::Net::WebSocket &ws,
		const std::string &data);

private:
	size_t m_maxMessageSize;
	GatewayCommunicator::Ptr m_gatewayCommunicator;
	GWSGatewayService::Ptr m_gatewayService;
	GatewayPeerVerifier::Ptr m_peerVerifier;
};

/**
 * @brief Factory for GWRequestHandler objects.
 */
class GWRequestHandlerFactory :
	public WebSocketRequestHandlerFactory {
public:
	typedef Poco::SharedPtr<GWRequestHandlerFactory> Ptr;

	GWRequestHandlerFactory();

	void setGatewayCommunicator(GatewayCommunicator::Ptr communicator);
	void setGatewayService(GWSGatewayService::Ptr service);
	void setVerifierFactory(SocketGatewayPeerVerifierFactory::Ptr factory);
	void setMaxMessageSize(int size);

protected:
	WebSocketRequestHandler *createWSHandler(
		const Poco::Net::HTTPServerRequest &request) override;

private:
	size_t m_maxMessageSize;
	GatewayCommunicator::Ptr m_gatewayCommunicator;
	GWSGatewayService::Ptr m_gatewayService;
	SocketGatewayPeerVerifierFactory::Ptr m_verifierFactory;
};

}

#endif
