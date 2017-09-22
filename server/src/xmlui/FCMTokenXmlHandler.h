#pragma once

#include "server/SessionManager.h"
#include "service/FCMTokenService.h"
#include "xmlui/SessionXmlHandler.h"

namespace BeeeOn {
namespace XmlUI {

class FCMTokenXmlHandler : public SessionXmlHandler {
public:
	FCMTokenXmlHandler(const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input,
			BeeeOn::Session::Ptr session,
			BeeeOn::FCMTokenService &tokenService);
	void handleInputImpl() override;
	void handleRegister(Poco::XML::Element *tokenNode);
	void handleUnregister(Poco::XML::Element *tokenNode);

private:
	FCMTokenService &m_tokenService;
};

class FCMTokenXmlHandlerResolver : public SessionXmlHandlerResolver {
public:
	FCMTokenXmlHandlerResolver();

	bool canHandle(const Poco::XML::Element &root);
	XmlRequestHandler *createHandler(
			const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input);

	void setFCMTokenService(FCMTokenService::Ptr service);
	void setSessionManager(SessionManager::Ptr manager);

private:
	FCMTokenService::Ptr m_tokenService;
	SessionManager::Ptr m_sessionManager;
};

}
}
