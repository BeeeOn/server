#ifndef BEEEON_UI_PLACE_HANDLER_H
#define BEEEON_UI_PLACE_HANDLER_H

#include <istream>
#include <Poco/Exception.h>

#include "di/InjectorTarget.h"
#include "server/RestHandler.h"
#include "service/PlaceService.h"
#include "service/IdentityService.h"
#include "policy/PlaceAccessPolicy.h"

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
			sendInvalidInput(context.response());
		}
	}

	const std::string handleCreate(std::istream &in,
			const VerifiedIdentityID &identityID);

	template <typename Context>
	void handleUpdate(Context &context)
	{
		std::istream &in = context.request().stream();
		const std::string &placeId = param(context, "placeId");

		m_accessPolicy->assureUpdate(context.userData(),
				Place(PlaceID::parse(placeId)));

		try {
			sendResultOrNotFound(
				context.response(),
				handleUpdate(in, placeId)
			);
		}
		catch (const Poco::Exception &e) {
			m_logger.log(e, __FILE__, __LINE__);
			sendInvalidInput(context.response());
		}
	}

	const std::string handleUpdate(std::istream &in,
			const std::string &placeId);

	template <typename Context>
	void handleGet(Context &context)
	{
		const std::string &placeId = param(context, "placeId");

		m_accessPolicy->assureGet(context.userData(),
				Place(PlaceID::parse(placeId)));

		try {
			sendResultOrNotFound(
				context.response(),
				handleGet(placeId)
			);
		}
		catch (const Poco::Exception &e) {
			m_logger.log(e, __FILE__, __LINE__);
			sendInvalidInput(context.response());
		}
	}

	const std::string handleGet(const std::string &placeId);

	template <typename Context>
	void handleDelete(Context &context)
	{
		const std::string &placeId = param(context, "placeId");

		m_accessPolicy->assureRemove(context.userData(),
				Place(PlaceID::parse(placeId)));

		try {
			sendResultOrNotFound(
				context.response(),
				handleDelete(placeId)
			);
		}
		catch (const Poco::Exception &e) {
			m_logger.log(e, __FILE__, __LINE__);
			sendInvalidInput(context.response());
		}
	}

	const std::string handleDelete(const std::string &placeId);

	void setPlaceService(PlaceService *service)
	{
		m_placeService = service;
	}

	void setIdentityService(IdentityService *service)
	{
		m_identityService = service;
	}

	void setAccessPolicy(PlaceAccessPolicy *accessPolicy)
	{
		m_accessPolicy = accessPolicy;
	}

private:
	PlaceService *m_placeService;
	IdentityService *m_identityService;
	PlaceAccessPolicy *m_accessPolicy;
};

}
}

#endif
