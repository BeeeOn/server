#include <map>
#include <Poco/StreamCopier.h>
#include <Poco/Net/NetException.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>

#include "service/AuthService.h"
#include "server/Route.h"
#include "model/JSONSerializer.h"
#include "ui/UIServerModule.h"
#include "Debug.h"

#include "ui/Auth.h"
#include "ui/PlaceHandler.h"
#include "ui/GatewayHandler.h"
#include "ui/LocationHandler.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::JSON;
using namespace Poco::Dynamic;
using namespace BeeeOn;

static void verifyAuthorized(const UIRequest &request,
		const UIRoute &route, UIServerModule &module)
{
	TRACE_FUNC();

	if (request.hasCredentials()) {
		string scheme;
		string authInfo;
		Poco::SharedPtr<ExpirableSession> session;

		request.getCredentials(scheme, authInfo);

		if (!module.sessionManager().lookup(authInfo, session))
			throw NotAuthenticatedException("Session not found");

		return;
	}

	throw NotAuthenticatedException("credentials not found");
}

void UIServerModule::injectionDone()
{
	m_factory->noRoute([](UIRouteContext &context) {
		RestHandler::sendNoRoute(context.response());
	});
	m_factory->noOperation([](UIRouteContext &context) {
		RestHandler::sendNoOperation(context.response());
	});
	m_factory->sessionVerifier(verifyAuthorized);
	m_factory->POST("/auth", [](UIRouteContext &context) {
		Auth::handlePost(context);
	}, false);
	m_factory->DELETE("/auth", [](UIRouteContext &context) {
		Auth::handleDelete(context);
	});
	m_factory->POST("/place", [](UIRouteContext &context) {
		UI::PlaceHandler::handlePost(context);
	});
	m_factory->PUT("/place/:placeId", [](UIRouteContext &context) {
		UI::PlaceHandler::handlePut(context);
	});
	m_factory->GET("/place/:placeId", [](UIRouteContext &context) {
		UI::PlaceHandler::handleGet(context);
	});
	m_factory->DELETE("/place/:placeId", [](UIRouteContext &context) {
		UI::PlaceHandler::handleDelete(context);
	});

	m_factory->POST("/place/:placeId/gateways/:gatewayId", [](UIRouteContext &context) {
		UI::GatewayHandler::handlePost(context);
	});
	m_factory->PUT("/place/:placeId/gateways/:gatewayId", [](UIRouteContext &context) {
		UI::GatewayHandler::handlePut(context);
	});
	m_factory->GET("/place/:placeId/gateways/:gatewayId", [](UIRouteContext &context) {
		UI::GatewayHandler::handleGet(context);
	});
	m_factory->DELETE("/place/:placeId/gateways/:gatewayId", [](UIRouteContext &context) {
		UI::GatewayHandler::handleDelete(context);
	});

	m_factory->POST("/place/:placeId/locations", [](UIRouteContext &context) {
		UI::LocationHandler::handlePost(context);
	});
	m_factory->GET("/place/:placeId/locations/:locationId", [](UIRouteContext &context) {
		UI::LocationHandler::handleGet(context);
	});
	m_factory->PUT("/place/:placeId/locations/:locationId", [](UIRouteContext &context) {
		UI::LocationHandler::handlePut(context);
	});
	m_factory->DELETE("/place/:placeId/locations/:locationId", [](UIRouteContext &context) {
		UI::LocationHandler::handleDelete(context);
	});
}

BEEEON_OBJECT(UIServerModule, BeeeOn::UIServerModule)
