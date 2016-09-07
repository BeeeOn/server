#include <Poco/Exception.h>

#include "ui/LocationHandler.h"
#include "ui/Serializing.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;
using namespace BeeeOn::UI;

void LocationHandler::handlePost(UIRouteContext &context)
{
	Logger &logger = context.userData().logger();
	UIRequest &request = context.request();
	UIResponse &response = context.response();
	LocationService &service = context.userData().locationService();

	try {
		handlePost(logger, request, response, service, context.params());
	} catch (const Exception &e) {
		logger.log(e, __FILE__, __LINE__);
		response.setStatusAndReason(UIResponse::HTTP_BAD_REQUEST);
	}
}

void LocationHandler::handlePost(Poco::Logger &logger,
		UIRequest &request,
		UIResponse &response,
		LocationService &service,
		const UIRoute::Params &params)
{
	Place place(PlaceID::parse(params.at("placeId")));

	Location location;
	deserialize(request.stream(), location);

	service.createIn(location, place);

	const string &result = serialize(location);
	response.sendBuffer(result.c_str(), result.size());
}

void LocationHandler::handlePut(UIRouteContext &context)
{
	Logger &logger = context.userData().logger();
	UIRequest &request = context.request();
	UIResponse &response = context.response();
	LocationService &service = context.userData().locationService();

	try {
		handlePut(logger, request, response, service, context.params());
	} catch (const Exception &e) {
		logger.log(e, __FILE__, __LINE__);
		response.setStatusAndReason(UIResponse::HTTP_BAD_REQUEST);
	}
}

void LocationHandler::handlePut(Poco::Logger &logger,
		UIRequest &request,
		UIResponse &response,
		LocationService &service,
		const UIRoute::Params &params)
{;
	Place place(PlaceID::parse(params.at("placeId")));
	Location location(LocationID::parse(params.at("locationId")));

	if (!service.fetchFrom(location, place)) {
		response.setStatusAndReason(UIResponse::HTTP_NOT_FOUND);
		return;
	}

	deserialize(request.stream(), location);
	if (!service.update(location)) {
		response.setStatusAndReason(UIResponse::HTTP_BAD_REQUEST);
		return;
	}

	const string &result = serialize(location);
	response.sendBuffer(result.c_str(), result.size());
}

void LocationHandler::handleGet(UIRouteContext &context)
{
	Logger &logger = context.userData().logger();
	UIRequest &request = context.request();
	UIResponse &response = context.response();
	LocationService &service = context.userData().locationService();

	try {
		handleGet(logger, request, response, service, context.params());
	} catch (const Exception &e) {
		logger.log(e, __FILE__, __LINE__);
		response.setStatusAndReason(UIResponse::HTTP_BAD_REQUEST);
	}
}

void LocationHandler::handleGet(Poco::Logger &logger,
		UIRequest &request,
		UIResponse &response,
		LocationService &service,
		const UIRoute::Params &params)
{
	Place place(PlaceID::parse(params.at("placeId")));
	Location location(LocationID::parse(params.at("locationId")));

	if (!service.fetchFrom(location, place)) {
		response.setStatusAndReason(UIResponse::HTTP_NOT_FOUND);
		return;
	}

	const string &result = serialize(location);
	response.sendBuffer(result.c_str(), result.size());
}

void LocationHandler::handleDelete(UIRouteContext &context)
{
	Logger &logger = context.userData().logger();
	UIRequest &request = context.request();
	UIResponse &response = context.response();
	LocationService &service = context.userData().locationService();

	try {
		handleDelete(logger, request, response, service, context.params());
	} catch (const Exception &e) {
		logger.log(e, __FILE__, __LINE__);
		response.setStatusAndReason(UIResponse::HTTP_BAD_REQUEST);
	}
}

void LocationHandler::handleDelete(Poco::Logger &logger,
		UIRequest &request,
		UIResponse &response,
		LocationService &service,
		const UIRoute::Params &params)
{
	Place place(PlaceID::parse(params.at("placeId")));
	Location location(LocationID::parse(params.at("locationId")));

	if (!service.fetchFrom(location, place)) {
		response.setStatusAndReason(UIResponse::HTTP_NOT_FOUND);
		return;
	}

	if (!service.remove(location)) {
		response.setStatusAndReason(UIResponse::HTTP_NOT_FOUND);
		return;
	}

	const string &result = serialize(location);
	response.sendBuffer(result.c_str(), result.size());
}
