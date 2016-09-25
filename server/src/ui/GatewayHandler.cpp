#include <Poco/Exception.h>

#include "ui/GatewayHandler.h"
#include "ui/Serializing.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;
using namespace BeeeOn::UI;

void GatewayHandler::handlePost(UIRouteContext &context)
{
	Logger &logger = context.userData().logger();
	UIRequest &request = context.request();
	UIResponse &response = context.response();
	GatewayService &service = context.userData().gatewayService();

	try {
		handlePost(logger, request, response, service, context.params());
	} catch (const Exception &e) {
		logger.log(e, __FILE__, __LINE__);
		response.setStatusAndReason(UIResponse::HTTP_BAD_REQUEST);
	}
}

void GatewayHandler::handlePost(Poco::Logger &logger,
		UIRequest &request,
		UIResponse &response,
		GatewayService &gatewayService,
		const UIRoute::Params &params)
{
	Place place(PlaceID::parse(params.at("placeId")));
	Gateway gateway(GatewayID::parse(params.at("gatewayId")));

	if (!gatewayService.fetch(gateway)) {
		response.setStatusAndReason(UIResponse::HTTP_NOT_FOUND);
		return;
	}

	deserialize(request.stream(), gateway);
	if (!gatewayService.assignAndUpdate(gateway, place)) {
		response.setStatusAndReason(UIResponse::HTTP_NOT_FOUND);
		return;
	}

	const string &result = serialize(gateway);
	response.sendBuffer(result.c_str(), result.size());
}

void GatewayHandler::handlePut(UIRouteContext &context)
{
	Logger &logger = context.userData().logger();
	UIRequest &request = context.request();
	UIResponse &response = context.response();
	GatewayService &service = context.userData().gatewayService();

	try {
		handlePut(logger, request, response, service, context.params());
	} catch (const Exception &e) {
		logger.log(e, __FILE__, __LINE__);
		response.setStatusAndReason(UIResponse::HTTP_BAD_REQUEST);
	}
}

void GatewayHandler::handlePut(Poco::Logger &logger,
		UIRequest &request,
		UIResponse &response,
		GatewayService &gatewayService,
		const UIRoute::Params &params)
{
	Gateway gateway(GatewayID::parse(params.at("gatewayId")));
	Place place(PlaceID::parse(params.at("placeId")));

	if (!gatewayService.fetchFromPlace(gateway, place)) {
		response.setStatusAndReason(UIResponse::HTTP_NOT_FOUND);
		return;
	}

	deserialize(request.stream(), gateway);
	if (!gatewayService.update(gateway)) {
		response.setStatusAndReason(UIResponse::HTTP_BAD_REQUEST);
		return;
	}

	const string &result = serialize(gateway);
	response.sendBuffer(result.c_str(), result.size());
}

void GatewayHandler::handleGet(UIRouteContext &context)
{
	Logger &logger = context.userData().logger();
	UIRequest &request = context.request();
	UIResponse &response = context.response();
	GatewayService &service = context.userData().gatewayService();

	try {
		handleGet(logger, request, response, service, context.params());
	} catch (const Exception &e) {
		logger.log(e, __FILE__, __LINE__);
		response.setStatusAndReason(UIResponse::HTTP_BAD_REQUEST);
	}
}

void GatewayHandler::handleGet(Poco::Logger &logger,
		UIRequest &request,
		UIResponse &response,
		GatewayService &gatewayService,
		const UIRoute::Params &params)
{
	Gateway gateway(GatewayID::parse(params.at("gatewayId")));
	Place place(PlaceID::parse(params.at("placeId")));

	if (!gatewayService.fetchFromPlace(gateway, place)) {
		response.setStatusAndReason(UIResponse::HTTP_NOT_FOUND);
		return;
	}

	const string &result = serialize(gateway);
	response.sendBuffer(result.c_str(), result.size());
}

void GatewayHandler::handleDelete(UIRouteContext &context)
{
	Logger &logger = context.userData().logger();
	UIRequest &request = context.request();
	UIResponse &response = context.response();
	GatewayService &service = context.userData().gatewayService();

	try {
		handleDelete(logger, request, response, service, context.params());
	} catch (const Exception &e) {
		logger.log(e, __FILE__, __LINE__);
		response.setStatusAndReason(UIResponse::HTTP_BAD_REQUEST);
	}
}

void GatewayHandler::handleDelete(Poco::Logger &logger,
		UIRequest &request,
		UIResponse &response,
		GatewayService &gatewayService,
		const UIRoute::Params &params)
{
	Gateway gateway(GatewayID::parse(params.at("gatewayId")));
	Place place(PlaceID::parse(params.at("placeId")));

	if (!gatewayService.unassign(gateway, place)) {
		response.setStatusAndReason(UIResponse::HTTP_NOT_FOUND);
		return;
	}

	const string &result = serialize(gateway);
	response.sendBuffer(result.c_str(), result.size());
}
