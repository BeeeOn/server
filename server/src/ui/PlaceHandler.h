#ifndef BEEEON_UI_PLACE_HANDLER_H
#define BEEEON_UI_PLACE_HANDLER_H

#include <istream>
#include <Poco/Exception.h>

#include "di/InjectorTarget.h"
#include "server/RestHandler.h"
#include "server/Session.h"
#include "service/PlaceService.h"
#include "service/IdentityService.h"
#include "model/User.h"

namespace BeeeOn {
namespace UI {

class PlaceHandler : public RestHandler, public AbstractInjectorTarget {
public:
	PlaceHandler();

	template <typename Context>
	void handleCreate(Context &context)
	{
		std::istream &in = context.request().stream();

		try {
			sendResult(
				context.response(),
				handleCreate(in, context.userData()->identityID())
			);
		}
		catch (const Poco::Exception &e) {
			m_logger.log(e, __FILE__, __LINE__);
			throw;
		}
	}

	const std::string handleCreate(std::istream &in,
			const VerifiedIdentityID &identityID);

	template <typename Context>
	void handleUpdate(Context &context)
	{
		std::istream &in = context.request().stream();
		const std::string &placeId = param(context, "placeId");
		const ExpirableSession::Ptr session = context.userData();

		try {
			sendResultOrNotFound(
				context.response(),
				handleUpdate(in, session->userID(), placeId)
			);
		}
		catch (const Poco::Exception &e) {
			m_logger.log(e, __FILE__, __LINE__);
			throw;
		}
	}

	const std::string handleUpdate(std::istream &in,
			const UserID &userId,
			const std::string &placeId);

	template <typename Context>
	void handleGet(Context &context)
	{
		const std::string &placeId = param(context, "placeId");
		const ExpirableSession::Ptr session = context.userData();

		try {
			sendResultOrNotFound(
				context.response(),
				handleGet(session->userID(), placeId)
			);
		}
		catch (const Poco::Exception &e) {
			m_logger.log(e, __FILE__, __LINE__);
			throw;
		}
	}

	const std::string handleGet(const UserID &userId,
			const std::string &placeId);

	template <typename Context>
	void handleDelete(Context &context)
	{
		const std::string &placeId = param(context, "placeId");
		const ExpirableSession::Ptr session = context.userData();

		try {
			sendResultOrNotFound(
				context.response(),
				handleDelete(session->userID(), placeId)
			);
		}
		catch (const Poco::Exception &e) {
			m_logger.log(e, __FILE__, __LINE__);
			throw;
		}
	}

	const std::string handleDelete(const UserID &userId,
			const std::string &placeId);

	template <typename Context>
	void handleGetAll(Context &context)
	{
		try {
			sendResultOrNotFound(
				context.response(),
				handleGetAll(context.userData()->userID())
			);
		}
		catch (const Poco::Exception &e) {
			m_logger.log(e, __FILE__, __LINE__);
			throw;
		}
	}

	const std::string handleGetAll(const UserID &userId);

	void setPlaceService(PlaceService *service)
	{
		m_placeService = service;
	}

	void setIdentityService(IdentityService *service)
	{
		m_identityService = service;
	}

private:
	PlaceService *m_placeService;
	IdentityService *m_identityService;
};

}
}

#endif
