#ifndef BEEEON_UI_LOCATION_HANDLER_H
#define BEEEON_UI_LOCATION_HANDLER_H

#include <istream>
#include <Poco/Exception.h>

#include "di/InjectorTarget.h"
#include "server/RestHandler.h"
#include "service/LocationService.h"

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
		const ExpirableSession::Ptr session = context.userData();

		try {
			sendResultOrNotFound(
				context.response(),
				handleCreate(in, session->userID(), placeId)
			);
		}
		catch (const Poco::Exception &e) {
			logger().log(e, __FILE__, __LINE__);
			throw;
		}
	}

	const std::string handleCreate(std::istream &in,
			const UserID &userId,
			const std::string &placeId);

	template <typename Context>
	void handleUpdate(Context &context)
	{
		std::istream &in = context.request().stream();
		const std::string placeId = param(context, "placeId");
		const std::string locationId = param(context, "locationId");
		const ExpirableSession::Ptr session = context.userData();

		try {
			sendResultOrNotFound(
				context.response(),
				handleUpdate(in, session->userID(),
					placeId, locationId)
			);
		}
		catch (const Poco::Exception &e) {
			logger().log(e, __FILE__, __LINE__);
			throw;
		}
	}

	const std::string handleUpdate(std::istream &in,
			const UserID &userId,
			const std::string &placeId,
			const std::string &locationId);

	template <typename Context>
	void handleGet(Context &context)
	{
		const std::string placeId = param(context, "placeId");
		const std::string locationId = param(context, "locationId");
		const ExpirableSession::Ptr session = context.userData();

		try {
			sendResultOrNotFound(
				context.response(),
				handleGet(session->userID(),
					placeId, locationId)
			);
		}
		catch (const Poco::Exception &e) {
			logger().log(e, __FILE__, __LINE__);
			throw;
		}
	}

	const std::string handleGet(
			const UserID &userId,
			const std::string &placeId,
			const std::string &locationId);

	template <typename Context>
	void handleDelete(Context &context)
	{
		const std::string placeId = param(context, "placeId");
		const std::string locationId = param(context, "locationId");
		const ExpirableSession::Ptr session = context.userData();

		try {
			sendResultOrNotFound(
				context.response(),
				handleDelete(session->userID(),
					placeId, locationId)
			);
		}
		catch (const Poco::Exception &e) {
			logger().log(e, __FILE__, __LINE__);
			throw;
		}
	}

	const std::string handleDelete(
			const UserID &userId,
			const std::string &placeId,
			const std::string &locationId);

	void setLocationService(LocationService *service)
	{
		m_locationService = service;
	}

private:
	LocationService *m_locationService;
};

}
}

#endif
