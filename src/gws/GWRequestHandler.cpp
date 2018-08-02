#include <Poco/Buffer.h>
#include <Poco/Clock.h>
#include <Poco/Logger.h>
#include <Poco/SharedPtr.h>
#include <Poco/JSON/Object.h>
#include <Poco/Net/HTTPServerRequestImpl.h>
#include <Poco/Net/WebSocket.h>

#include "di/Injectable.h"
#include "gwmessage/GWGatewayRegister.h"
#include "gwmessage/GWGatewayAccepted.h"
#include "gws/GWRequestHandler.h"
#include "util/Sanitize.h"

BEEEON_OBJECT_BEGIN(BeeeOn, GWRequestHandlerFactory)
BEEEON_OBJECT_CASTABLE(WebSocketRequestHandlerFactory)
BEEEON_OBJECT_PROPERTY("gatewayCommunicator", &GWRequestHandlerFactory::setGatewayCommunicator)
BEEEON_OBJECT_PROPERTY("maxMessageSize", &GWRequestHandlerFactory::setMaxMessageSize)
BEEEON_OBJECT_PROPERTY("gatewayService", &GWRequestHandlerFactory::setGatewayService)
BEEEON_OBJECT_PROPERTY("verifierFactory", &GWRequestHandlerFactory::setVerifierFactory)
BEEEON_OBJECT_END(BeeeOn, GWRequestHandlerFactory)

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

GWRequestHandler::GWRequestHandler(
		size_t maxMessageSize,
		GatewayCommunicator::Ptr communicator,
		GWSGatewayService::Ptr service,
		GatewayPeerVerifier::Ptr peerVerifier):
	m_maxMessageSize(maxMessageSize),
	m_gatewayCommunicator(communicator),
	m_gatewayService(service),
	m_peerVerifier(peerVerifier)
{
}

void GWRequestHandler::handle(WebSocket &ws)
{
	Thread::current()->setName("ws");

	Poco::Buffer<char> buffer(m_maxMessageSize);
	int flags;

	int ret = ws.receiveFrame(buffer.begin(), buffer.size(), flags);
	if (ret <= 0 || (flags & WebSocket::FRAME_OP_CLOSE)) {
		if (logger().debug()) {
			logger().debug(ws.peerAddress().toString()
				+ " connection closed",
				__FILE__, __LINE__);
		}
		return;
	}

	logger().information("connection from "
			+ ws.peerAddress().toString(),
			__FILE__, __LINE__);

	string data(buffer.begin(), ret);
	processPayload(ws, data);
}

void GWRequestHandler::processPayload(
		WebSocket &ws,
		string data)
{
	if (logger().trace())
		logger().trace(data);

	GWMessage::Ptr msg = GWMessage::fromJSON(data);
	GWGatewayRegister::Ptr registerMsg = msg.cast<GWGatewayRegister>();

	if (registerMsg.isNull()) {
		logger().warning("invalid message from "
			+ ws.peerAddress().toString() + ":\n"
			+ msg->toString(),
			__FILE__, __LINE__);
		return;
	}

	Gateway gateway(registerMsg->gatewayID());

	Thread::current()->setName("ws-" + gateway);

	m_peerVerifier->verifyPeer(gateway);

	GatewayStatus status;
	status.setVersion(Sanitize::common(registerMsg->version()));
	status.setIPAddress(registerMsg->ipAddress());

	if (!m_gatewayService->registerGateway(status, gateway)) {
		logger().error("failed to register gateway "
				+ gateway, __FILE__, __LINE__);
		return;
	}

	m_gatewayCommunicator->addGateway(gateway.id(), ws);

	data = GWGatewayAccepted().toString();
	ws.sendFrame(data.c_str(), data.length());
}

GWRequestHandlerFactory::GWRequestHandlerFactory():
	m_maxMessageSize(256)
{
}

void GWRequestHandlerFactory::setGatewayCommunicator(GatewayCommunicator::Ptr communicator)
{
	m_gatewayCommunicator = communicator;
}

void GWRequestHandlerFactory::setGatewayService(GWSGatewayService::Ptr service)
{
	m_gatewayService = service;
}

void GWRequestHandlerFactory::setVerifierFactory(SocketGatewayPeerVerifierFactory::Ptr factory)
{
	m_verifierFactory = factory;
}

void GWRequestHandlerFactory::setMaxMessageSize(int size)
{
	if (size < 0)
		throw InvalidArgumentException("size must be non-negative");

	m_maxMessageSize = size;
}

WebSocketRequestHandler *GWRequestHandlerFactory::createWSHandler(
	const HTTPServerRequest &request)
{
	GatewayPeerVerifier::Ptr verifier;
	const HTTPServerRequestImpl *impl = nullptr;

	impl = dynamic_cast<const HTTPServerRequestImpl *>(&request);
	if (impl == nullptr) {
		throw IllegalStateException(
			"given request instance is not inherited from HTTPServerRequestImpl");
	}

	verifier = m_verifierFactory->preverifyAndCreate(
			const_cast<HTTPServerRequestImpl *>(impl)->socket());

	return new GWRequestHandler(
		m_maxMessageSize,
		m_gatewayCommunicator,
		m_gatewayService,
		verifier
	);
}
