#pragma once

#include "xmlui/SessionXmlHandler.h"

namespace BeeeOn {

class IdentityService;

namespace XmlUI {

class ProfileXmlHandler : public SessionXmlHandler {
public:
	ProfileXmlHandler(const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input,
			BeeeOn::Session::Ptr session,
			IdentityService &identityService);
	void handleInputImpl() override;
	void handleMyProfile();

private:
	IdentityService &m_identityService;
};

class ProfileXmlHandlerResolver : public SessionXmlHandlerResolver {
public:
	ProfileXmlHandlerResolver();

	bool canHandle(const Poco::XML::Element &root);
	XmlRequestHandler *createHandler(
			const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input);

	void setIdentityService(IdentityService *service)
	{
		m_identityService = service;
	}

	void setSessionManager(BeeeOn::SessionManager *manager)
	{
		m_sessionManager = manager;
	}

private:
	IdentityService *m_identityService;
	SessionManager *m_sessionManager;
};

}
}
