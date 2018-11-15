#pragma once

#include "rest/JSONRestHandler.h"
#include "service/IdentityService.h"
#include "service/RoleService.h"

namespace BeeeOn {
namespace RestUI {

class RoleRestHandler : public JSONRestHandler {

public:
	RoleRestHandler();

	void setRoleService(RoleService::Ptr service);
	void setIdentityService(IdentityService::Ptr service);

	/**
	 * @brief List all roles associated with the given gateway.
	 *
	 * - usual responses: 200
	 * - example output (data):
	 * <pre>
	 * [
	 *   {
	 *     "id": "7ac2d962-72d0-496a-a8c3-d44074fef561",
	 *     "identity_email": "user1@example.org",
	 *     "access_level": 30,
	 *     "created": 1536682053
	 *   },
	 *   {
	 *     "id": "68e41151-4d22-4509-bf04-4a8cc6b49e67",
	 *     "identity_email": "joe@example.org",
	 *     "access_level": 10,
	 *     "created": 1536672181
	 *   }
	 * ]
	 * </pre>
	 */
	void list(RestFlow &flow);

	/**
	 * @brief Show details of a selected role.
	 *
	 * - usual responses: 200, 404
	 * - example output (data):
	 * <pre>
	 * {
	 *   "id": "2bfd5682-ee89-409b-a27a-38e3e16fcc35",
	 *   "identity_email": "john@example.org",
	 *   "access_level": 10,
	 *   "created": 1536682231,
	 *   "user": {
	 *     "first_name": "John",
	 *     "last_name": "Smith"
	 *   }
	 * }
	 * </pre>
	 */
	void detail(RestFlow &flow);

	/**
	 * @brief Redirect to detail of the current role associated with
	 * the given gateway. The result is role of the current user.
	 *
	 * - Location: {ROLE_CURRENT_DETAIL}
	 * - usual responses: 303, 404
	 */
	void current(RestFlow &flow);

	/**
	 * @brief Create a new role for the given gateway. The user of
	 * the role is invited to use the associated gateway.
	 *
	 * - Location: {ROLE_CREATED_DETAIL}
	 * - usual responses: 201
	 * - example input:
	 * <pre>
	 * {
	 *   "identity_email": "john@example.org",
	 *   "access_level": 20
	 * }
	 * </pre>
	 */
	void create(RestFlow &flow);
	void update(RestFlow &flow);
	void remove(RestFlow &flow);

private:
	RoleService::Ptr m_roleService;
	IdentityService::Ptr m_identityService;
};

}
}
