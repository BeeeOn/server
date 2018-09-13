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

	void list(RestFlow &flow);
	void detail(RestFlow &flow);
	void current(RestFlow &flow);
	void create(RestFlow &flow);
	void update(RestFlow &flow);
	void remove(RestFlow &flow);

private:
	RoleService::Ptr m_roleService;
	IdentityService::Ptr m_identityService;
};

}
}
