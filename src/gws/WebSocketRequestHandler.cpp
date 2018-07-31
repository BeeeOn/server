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
#include "gws/WebSocketRequestHandler.h"
#include "util/Sanitize.h"

BEEEON_OBJECT_BEGIN(BeeeOn, WebSocketRequestHandlerFactory)
BEEEON_OBJECT_PROPERTY("gatewayCommunicator", &WebSocketRequestHandlerFactory::setGatewayCommunicator)
BEEEON_OBJECT_PROPERTY("maxMessageSize", &WebSocketRequestHandlerFactory::setMaxMessageSize)
BEEEON_OBJECT_PROPERTY("gatewayService", &WebSocketRequestHandlerFactory::setGatewayService)
BEEEON_OBJECT_PROPERTY("verifierFactory", &WebSocketRequestHandlerFactory::setVerifierFactory)
BEEEON_OBJECT_END(BeeeOn, WebSocketRequestHandlerFactory)

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

WebSocketRequestHandler::WebSocketRequestHandler(
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

void WebSocketRequestHandler::run()
{
	const Clock started;

	try {
		Thread::current()->setName("ws");

		WebSocket ws(request(), response());

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
	catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
	}
	catch (const exception &e) {
		logger().critical(e.what(), __FILE__, __LINE__);
	}
	catch (...) {
		logger().critical("unknown error, cought '...'", __FILE__, __LINE__);
	}

	if (logger().information()) {
		logger().information("duration: "
			+ to_string(started.elapsed()) + "us",
			__FILE__, __LINE__);
	}
}

void WebSocketRequestHandler::processPayload(
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

	data = GWGatewayAccepted().toString();
	ws.sendFrame(data.c_str(), data.length());

	m_gatewayCommunicator->addGateway(gateway.id(), ws);
}

WebSocketRequestHandlerFactory::WebSocketRequestHandlerFactory():
	m_maxMessageSize(256)
{
}

void WebSocketRequestHandlerFactory::setGatewayCommunicator(GatewayCommunicator::Ptr communicator)
{
	m_gatewayCommunicator = communicator;
}

void WebSocketRequestHandlerFactory::setGatewayService(GWSGatewayService::Ptr service)
{
	m_gatewayService = service;
}

void WebSocketRequestHandlerFactory::setVerifierFactory(SocketGatewayPeerVerifierFactory::Ptr factory)
{
	m_verifierFactory = factory;
}

void WebSocketRequestHandlerFactory::setMaxMessageSize(int size)
{
	if (size < 0)
		throw InvalidArgumentException("size must be non-negative");

	m_maxMessageSize = size;
}

HTTPRequestHandler *WebSocketRequestHandlerFactory::createRequestHandler(
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

	return new WebSocketRequestHandler(
		m_maxMessageSize,
		m_gatewayCommunicator,
		m_gatewayService,
		verifier
	);
}
