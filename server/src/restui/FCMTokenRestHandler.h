#pragma once

#include <string>

#include "rest/JSONRestHandler.h"
#include "service/FCMTokenService.h"

namespace BeeeOn {
namespace RestUI {

class FCMTokenRestHandler : public JSONRestHandler {
public:
	FCMTokenRestHandler();

	void setFCMTokenService(FCMTokenService::Ptr service);
	void setSenderID(const std::string &senderID);

	void listServices(RestFlow &flow);
	void registerToken(RestFlow &flow);
	void unregisterToken(RestFlow &flow);

protected:
	FCMToken extractToken(RestFlow &flow) const;

private:
	FCMTokenService::Ptr m_service;
	std::string m_senderID;
};

}
}
