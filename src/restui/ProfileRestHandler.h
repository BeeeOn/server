#pragma once

#include "rest/JSONRestHandler.h"
#include "service/IdentityService.h"

namespace BeeeOn {
namespace RestUI {

class ProfileRestHandler : public JSONRestHandler {
public:
	ProfileRestHandler();

	void setIdentityService(IdentityService::Ptr service);

	void detail(RestFlow &flow);
	void current(RestFlow &flow);
	void list(RestFlow &flow);

private:
	IdentityService::Ptr m_identityService;
};

}
}
