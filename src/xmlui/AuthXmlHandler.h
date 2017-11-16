#ifndef BEEEON_XMLUI_AUTH_XML_HANDLER_H
#define BEEEON_XMLUI_AUTH_XML_HANDLER_H

#include "xmlui/AbstractXmlHandler.h"
#include "service/AuthService.h"

namespace BeeeOn {
namespace XmlUI {

class AuthXmlHandler : public AbstractXmlHandler {
public:
	AuthXmlHandler(const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input,
			BeeeOn::AuthService &service);

	void handleInputImpl() override;

private:
	BeeeOn::AuthService &m_authService;
};

class AuthXmlHandlerResolver : public AbstractXmlHandlerResolver {
public:
	AuthXmlHandlerResolver();

	bool canHandle(const Poco::XML::Element &input);
	XmlRequestHandler *createHandler(
			const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input);

	void setAuthService(BeeeOn::AuthService *service)
	{
		m_authService = service;
	}

private:
	BeeeOn::AuthService *m_authService;
};

}
}

#endif
