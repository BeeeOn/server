#ifndef BEEEON_XMLUI_PROFILE_XML_HANDLER_H
#define BEEEON_XMLUI_PROFILE_XML_HANDLER_H

#include "xmlui/AbstractXmlHandler.h"

namespace BeeeOn {

class IdentityService;

namespace XmlUI {

class ProfileXmlHandler : public AbstractXmlHandler {
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

class ProfileXmlHandlerResolver : public AbstractXmlHandlerResolver {
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

#endif
