#include <Poco/Exception.h>

#include "ui/PlaceHandler.h"
#include "ui/Serializing.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;
using namespace BeeeOn::UI;

void PlaceHandler::handlePost(UIRouteContext &context)
{
	Logger &logger = context.userData().logger();
	UIRequest &request = context.request();
	UIResponse &response = context.response();
	PlaceService &service = context.userData().placeService();

	try {
		handlePost(logger, request, response, service);
	} catch (const Exception &e) {
		logger.log(e, __FILE__, __LINE__);
		response.setStatusAndReason(UIResponse::HTTP_BAD_REQUEST);
	}
}

void PlaceHandler::handlePost(Poco::Logger &logger,
		UIRequest &request,
		UIResponse &response,
		PlaceService &placeService)
{
	Place place;
	deserialize(request.stream(), place);

	placeService.create(place);

	const string &result = serialize(place);
	response.sendBuffer(result.c_str(), result.size());
}

void PlaceHandler::handlePut(UIRouteContext &context)
{
	Logger &logger = context.userData().logger();
	UIRequest &request = context.request();
	UIResponse &response = context.response();
	PlaceService &service = context.userData().placeService();

	try {
		handlePut(logger, request, response, service,
				context.params());
	} catch (const Exception &e) {
		logger.log(e, __FILE__, __LINE__);
		response.setStatusAndReason(UIResponse::HTTP_BAD_REQUEST);
	}
}

void PlaceHandler::handlePut(Poco::Logger &logger,
		UIRequest &request,
		UIResponse &response,
		PlaceService &placeService,
		const UIRoute::Params &params)
{
	Place place(PlaceID::parse(params.at("placeId")));

	if (!placeService.fetch(place)) {
		response.setStatusAndReason(UIResponse::HTTP_NOT_FOUND);
		return;
	}

	deserialize(request.stream(), place);

	placeService.update(place);

	const string &result = serialize(place);
	response.sendBuffer(result.c_str(), result.size());
}

void PlaceHandler::handleGet(UIRouteContext &context)
{
	Logger &logger = context.userData().logger();
	UIRequest &request = context.request();
	UIResponse &response = context.response();
	PlaceService &service = context.userData().placeService();

	try {
		handleGet(logger, request, response, service,
				context.params());
	} catch (const Exception &e) {
		logger.log(e, __FILE__, __LINE__);
		response.setStatusAndReason(UIResponse::HTTP_BAD_REQUEST);
	}
}

void PlaceHandler::handleGet(Poco::Logger &logger,
		UIRequest &request,
		UIResponse &response,
		PlaceService &placeService,
		const UIRoute::Params &params)
{
	Place place(PlaceID::parse(params.at("placeId")));

	if (!placeService.fetch(place)) {
		response.setStatusAndReason(UIResponse::HTTP_NOT_FOUND);
		return;
	}

	const string &result = serialize(place);
	response.sendBuffer(result.c_str(), result.size());
}

void PlaceHandler::handleDelete(UIRouteContext &context)
{
	Logger &logger = context.userData().logger();
	UIRequest &request = context.request();
	UIResponse &response = context.response();
	PlaceService &service = context.userData().placeService();

	try {
		handleDelete(logger, request, response, service,
				context.params());
	} catch (const Exception &e) {
		logger.log(e, __FILE__, __LINE__);
		response.setStatusAndReason(UIResponse::HTTP_BAD_REQUEST);
	}
}

void PlaceHandler::handleDelete(Poco::Logger &logger,
		UIRequest &request,
		UIResponse &response,
		PlaceService &placeService,
		const UIRoute::Params &params)
{
	Place place(PlaceID::parse(params.at("placeId")));

	if (!placeService.remove(place)) {
		response.setStatusAndReason(UIResponse::HTTP_NOT_FOUND);
		return;
	}

	const string &result = serialize(place);
	response.sendBuffer(result.c_str(), result.size());
}
