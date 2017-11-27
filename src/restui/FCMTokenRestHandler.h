#pragma once

#include <string>

#include "fcm/FCMReceiverConfigurable.h"
#include "rest/JSONRestHandler.h"
#include "service/FCMTokenService.h"

namespace BeeeOn {
namespace RestUI {

class FCMTokenRestHandler : public JSONRestHandler, public FCMReceiverConfigurable {
public:
	FCMTokenRestHandler();

	void setFCMTokenService(FCMTokenService::Ptr service);

	void listServices(RestFlow &flow);
	void registerToken(RestFlow &flow);
	void unregisterToken(RestFlow &flow);

protected:
	FCMToken extractToken(RestFlow &flow) const;

private:
	FCMTokenService::Ptr m_service;
};

}
}
