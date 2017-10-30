#pragma once

#include "rest/JSONRestHandler.h"
#include "service/FCMTokenService.h"

namespace BeeeOn {
namespace RestUI {

class FCMTokenRestHandler : public JSONRestHandler {
public:
	FCMTokenRestHandler();

	void setFCMTokenService(FCMTokenService::Ptr service);

	void registerToken(RestFlow &flow);
	void unregisterToken(RestFlow &flow);

protected:
	FCMToken extractToken(RestFlow &flow) const;

private:
	FCMTokenService::Ptr m_service;
};

}
}
