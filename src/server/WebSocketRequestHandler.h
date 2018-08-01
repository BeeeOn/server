#pragma once

#include <Poco/SharedPtr.h>
#include <Poco/Net/AbstractHTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/WebSocket.h>

#include "util/Loggable.h"

namespace BeeeOn {

/**
 * @brief Abstract WebSocketRequestHandler intended to simplify
 * construction of WebSocket initialization process.
 */
class WebSocketRequestHandler :
	public Poco::Net::AbstractHTTPRequestHandler,
	protected Loggable {
public:
	WebSocketRequestHandler();
	~WebSocketRequestHandler();

	void run() override;

protected:
	virtual void handle(Poco::Net::WebSocket &ws) = 0;

};

/**
 * @brief Abstract class to denote a factory of WebSocketRequestHandler class.
 */
class WebSocketRequestHandlerFactory :
	public Poco::Net::HTTPRequestHandlerFactory,
	protected Loggable {
public:
	typedef Poco::SharedPtr<WebSocketRequestHandlerFactory> Ptr;

	WebSocketRequestHandlerFactory();
	~WebSocketRequestHandlerFactory();

	Poco::Net::HTTPRequestHandler *createRequestHandler(
		const Poco::Net::HTTPServerRequest &request) override;

protected:
	virtual WebSocketRequestHandler *createWSHandler(
		const Poco::Net::HTTPServerRequest &request) = 0;
};

}
