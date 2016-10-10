#ifndef BEEEON_UI_GATEWAY_HANDLER_H
#define BEEEON_UI_GATEWAY_HANDLER_H

#include <istream>
#include <Poco/Exception.h>

#include "di/InjectorTarget.h"
#include "server/Session.h"
#include "server/RestHandler.h"
#include "service/GatewayService.h"
#include "policy/GatewayAccessPolicy.h"

namespace BeeeOn {
namespace UI {

class GatewayHandler : public RestHandler, public AbstractInjectorTarget {
public:
	GatewayHandler();

	/**
	  * Assign a gateway to a place both specified in URI.
	  * If new values for gateway are included they will be updated as well
	  * (i.e. name,location, altitude).
	  */
	template <typename Context>
	void handleAssign(Context &context)
	{
		std::istream &in = context.request().stream();
		const std::string placeId = param(context, "placeId");
		const std::string gatewayId = param(context, "gatewayId");
		const ExpirableSession::Ptr session = context.userData();

		try {
			sendResultOrNotFound(
				context.response(),
				handleAssign(in, session->userID(),
					placeId, gatewayId)
			);
		}
		catch (const Poco::InvalidAccessException &e) {
			m_logger.log(e, __FILE__, __LINE__);
			throw e;
		}
		catch (const Poco::Exception &e) {
			m_logger.log(e, __FILE__, __LINE__);
			sendInvalidInput(context.response());
		}
	}

	const std::string handleAssign(std::istream &in,
			const UserID &userId,
			const std::string &placeId,
			const std::string &gatewayId);

	/**
	  * Update gateway properties.
	  */
	template <typename Context>
	void handleUpdate(Context &context)
	{
		std::istream &in = context.request().stream();
		const std::string &placeId = param(context, "placeId");
		const std::string &gatewayId = param(context, "gatewayId");
		const ExpirableSession::Ptr session = context.userData();

		try {
			sendResultOrNotFound(
				context.response(),
				handleUpdate(in, session->userID(),
					placeId, gatewayId)
			);
		}
		catch (const Poco::InvalidAccessException &e) {
			m_logger.log(e, __FILE__, __LINE__);
			throw e;
		}
		catch (const Poco::Exception &e) {
			m_logger.log(e, __FILE__, __LINE__);
			sendInvalidInput(context.response());
		}
	}

	const std::string handleUpdate(std::istream &in,
			const UserID &userId,
			const std::string &placeId,
			const std::string &gatewayId);

	/**
	  * Fetch specified gateway.
	  */
	template <typename Context>
	void handleGet(Context &context)
	{
		const std::string &placeId = param(context, "placeId");
		const std::string &gatewayId = param(context, "gatewayId");
		const ExpirableSession::Ptr session = context.userData();

		try {
			sendResultOrNotFound(
				context.response(),
				handleGet(session->userID(),
					placeId, gatewayId)
			);
		}
		catch (const Poco::InvalidAccessException &e) {
			m_logger.log(e, __FILE__, __LINE__);
			throw e;
		}
		catch (const Poco::Exception &e) {
			m_logger.log(e, __FILE__, __LINE__);
			sendInvalidInput(context.response());
		}
	}

	const std::string handleGet(
			const UserID &userId,
			const std::string &placeId,
			const std::string &gatewayId);

	/**
	  * Unassign gateway from place. This does not delete the gateway from DB.
	  */
	template <typename Context>
	void handleDelete(Context &context)
	{
		const std::string &placeId = param(context, "placeId");
		const std::string &gatewayId = param(context, "gatewayId");
		const ExpirableSession::Ptr session = context.userData();

		try {
			sendResultOrNotFound(
				context.response(),
				handleDelete(session->userID(),
					placeId, gatewayId)
			);
		}
		catch (const Poco::InvalidAccessException &e) {
			m_logger.log(e, __FILE__, __LINE__);
			throw e;
		}
		catch (const Poco::Exception &e) {
			m_logger.log(e, __FILE__, __LINE__);
			sendInvalidInput(context.response());
		}
	}

	const std::string handleDelete(
			const UserID &userId,
			const std::string &placeId,
			const std::string &gatewayId);

	void setGatewayService(GatewayService *service)
	{
		m_gatewayService = service;
	}

	void setAccessPolicy(GatewayAccessPolicy *policy)
	{
		m_accessPolicy = policy;
	}

private:
	GatewayService *m_gatewayService;
	GatewayAccessPolicy *m_accessPolicy;
};

}
}

#endif
