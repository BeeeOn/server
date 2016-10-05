#ifndef BEEEON_UI_LOCATION_HANDLER_H
#define BEEEON_UI_LOCATION_HANDLER_H

#include <istream>
#include <Poco/Exception.h>

#include "di/InjectorTarget.h"
#include "server/RestHandler.h"
#include "service/LocationService.h"
#include "policy/LocationAccessPolicy.h"

namespace BeeeOn {
namespace UI {

class LocationHandler : public RestHandler, public AbstractInjectorTarget {
public:
	LocationHandler();

	template <typename Context>
	void handleCreate(Context &context)
	{
		std::istream &in = context.request().stream();
		const std::string placeId = param(context, "placeId");

		m_accessPolicy->assureCreateLocation(context.userData(),
				Place(PlaceID::parse(placeId)));

		try {
			sendResultOrNotFound(
				context.response(),
				handleCreate(in, placeId)
			);
		}
		catch (const Poco::Exception &e) {
			m_logger.log(e, __FILE__, __LINE__);
			sendInvalidInput(context.response());
		}
	}

	const std::string handleCreate(std::istream &in,
			const std::string &placeId);

	template <typename Context>
	void handleUpdate(Context &context)
	{
		std::istream &in = context.request().stream();
		const std::string placeId = param(context, "placeId");
		const std::string locationId = param(context, "locationId");

		m_accessPolicy->assureUpdate(context.userData(),
				Location(LocationID::parse(locationId)));

		try {
			sendResultOrNotFound(
				context.response(),
				handleUpdate(in, placeId, locationId)
			);
		}
		catch (const Poco::Exception &e) {
			m_logger.log(e, __FILE__, __LINE__);
			sendInvalidInput(context.response());
		}
	}

	const std::string handleUpdate(std::istream &in,
			const std::string &placeId,
			const std::string &locationId);

	template <typename Context>
	void handleGet(Context &context)
	{
		const std::string placeId = param(context, "placeId");
		const std::string locationId = param(context, "locationId");

		m_accessPolicy->assureGet(context.userData(),
				Location(LocationID::parse(locationId)));

		try {
			sendResultOrNotFound(
				context.response(),
				handleGet(placeId, locationId)
			);
		}
		catch (const Poco::Exception &e) {
			m_logger.log(e, __FILE__, __LINE__);
			sendInvalidInput(context.response());
		}
	}

	const std::string handleGet(
			const std::string &placeId,
			const std::string &locationId);

	template <typename Context>
	void handleDelete(Context &context)
	{
		const std::string placeId = param(context, "placeId");
		const std::string locationId = param(context, "locationId");

		m_accessPolicy->assureRemove(context.userData(),
				Location(LocationID::parse(locationId)));

		try {
			sendResultOrNotFound(
				context.response(),
				handleDelete(placeId, locationId)
			);
		}
		catch (const Poco::Exception &e) {
			m_logger.log(e, __FILE__, __LINE__);
			sendInvalidInput(context.response());
		}
	}

	const std::string handleDelete(
			const std::string &placeId,
			const std::string &locationId);

	void setLocationService(LocationService *service)
	{
		m_locationService = service;
	}

	void setAccessPolicy(LocationAccessPolicy *policy)
	{
		m_accessPolicy = policy;
	}

private:
	LocationService *m_locationService;
	LocationAccessPolicy *m_accessPolicy;
};

}
}

#endif
