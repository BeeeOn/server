#pragma once

#include "rest/JSONRestHandler.h"
#include "service/IdentityService.h"

namespace BeeeOn {
namespace RestUI {

class ProfileRestHandler : public JSONRestHandler {
public:
	ProfileRestHandler();

	void setIdentityService(IdentityService::Ptr service);

	/**
	 * @brief Show details of a profiles given by ID.
	 *
	 * - example output (data):
	 * <pre>
	 * {
	 *   "id": "282d100d-a35c-4a93-a45a-1d741f381f12",
	 *   "first_name": "John",
	 *   "last_name": "John",
	 *   "locale": "cs_CZ",
	 *   "locale_display_name": "Česká republika",
	 *   "email": "john.smith@example.org",
	 *   "picture_url": "",
	 *   "provider": "google",
	 *   "provider_display_name": "Google"
	 * }
	 * </pre>
	 */
	void detail(RestFlow &flow);

	/**
	 * @brief Redirect to detail of the current profile.
	 *
	 * - Location: {PROFILE_CURRENT_DETAIL}
	 * - usual responses: 303
	 */
	void current(RestFlow &flow);

	/**
	 * @brief List all profiles available for the current user.
	 * User profiles are usually sharing the same e-mail address
	 * but the profile identities are verified by different
	 * identity providers (Google, Facebook, etc.).
	 */
	void list(RestFlow &flow);

private:
	IdentityService::Ptr m_identityService;
};

}
}
