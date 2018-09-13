#pragma once

#include <Poco/Timespan.h>

#include "l10n/TimeZoneProvider.h"
#include "service/GatewayService.h"
#include "xmlui/SessionXmlHandler.h"

namespace BeeeOn {
namespace XmlUI {

class GatewayXmlHandler : public SessionXmlHandler {
public:
	GatewayXmlHandler(const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input,
			BeeeOn::Session::Ptr session,
			const Poco::Timespan &duration,
			BeeeOn::GatewayService &gatewayService,
			TimeZoneProvider::Ptr timeZoneProvider);
	void handleInputImpl() override;
	void handleRegister(Poco::XML::Element *gatewayNode);
	void handleUnregister(Poco::XML::Element *gatewayNode);
	void handleListen(Poco::XML::Element *gatewayNode);
	void handleGet(Poco::XML::Element *gatewayNode);
	void handleUpdate(Poco::XML::Element *gatewayNode);
	void handleGetAll();

private:
	Poco::Timespan m_scanDuration;
	GatewayService &m_gatewayService;
	TimeZoneProvider::Ptr m_timeZoneProvider;
};

class GatewayXmlHandlerResolver : public SessionXmlHandlerResolver {
public:
	GatewayXmlHandlerResolver();

	bool canHandle(const Poco::XML::Element &root);
	XmlRequestHandler *createHandler(
			const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input);

	void setScanDuration(const Poco::Timespan &duration);

	void setGatewayService(BeeeOn::GatewayService *service)
	{
		m_gatewayService = service;
	}

	void setSessionManager(BeeeOn::SessionManager *manager)
	{
		m_sessionManager = manager;
	}

	void setTimeZoneProvider(TimeZoneProvider::Ptr provider);

private:
	Poco::Timespan m_scanDuration;
	BeeeOn::GatewayService *m_gatewayService;
	BeeeOn::SessionManager *m_sessionManager;
	TimeZoneProvider::Ptr m_timeZoneProvider;
};

}
}
