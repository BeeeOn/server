#include <Poco/Buffer.h>
#include <Poco/Logger.h>
#include <Poco/SharedPtr.h>
#include <Poco/JSON/Object.h>
#include <Poco/Net/WebSocket.h>

#include "gwmessage/GWGatewayRegister.h"
#include "gwmessage/GWGatewayAccepted.h"
#include "gws/WebSocketRequestHandler.h"
#include "util/Sanitize.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

void WebSocketRequestHandler::handleRequest(
	HTTPServerRequest &request, HTTPServerResponse &response)
{
	try {
		Thread::current()->setName("ws");

		WebSocket ws(request, response);

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
	catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
	}
	catch (const exception &e) {
		logger().critical(e.what(), __FILE__, __LINE__);
	}
	catch (...) {
		logger().critical("unknown error, cought '...'", __FILE__, __LINE__);
	}
}

