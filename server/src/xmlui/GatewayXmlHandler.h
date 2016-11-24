#ifndef BEEEON_XMLUI_GATEWAY_XML_HANDLER_H
#define BEEEON_XMLUI_GATEWAY_XML_HANDLER_H

#include "service/GatewayService.h"
#include "xmlui/AbstractXmlHandler.h"

namespace BeeeOn {
namespace XmlUI {

class GatewayXmlHandler : public AbstractXmlHandler {
public:
	GatewayXmlHandler(const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input,
			BeeeOn::ExpirableSession::Ptr session,
			BeeeOn::GatewayService &gatewayService);
	void handleInputImpl() override;
	void handleRegister(Poco::XML::Element *gatewayNode);
	void handleUnregister(Poco::XML::Element *gatewayNode);
	void handleListen(Poco::XML::Element *gatewayNode);
	void handleGet(Poco::XML::Element *gatewayNode);
	void handleUpdate(Poco::XML::Element *gatewayNode);
	void handleGetAll();

private:
	GatewayService &m_gatewayService;
};

class GatewayXmlHandlerResolver : public AbstractXmlHandlerResolver {
public:
	GatewayXmlHandlerResolver();

	bool canHandle(const Poco::XML::Element &root);
	XmlRequestHandler *createHandler(
			const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input);

	void setGatewayService(BeeeOn::GatewayService *service)
	{
		m_gatewayService = service;
	}

	void setSessionManager(BeeeOn::SessionManager *manager)
	{
		m_sessionManager = manager;
	}

private:
	BeeeOn::GatewayService *m_gatewayService;
	BeeeOn::SessionManager *m_sessionManager;
};

}
}

#endif
