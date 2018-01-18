#ifndef BEEEON_XMLUI_SENSOR_XML_HANDLER_H
#define BEEEON_XMLUI_SENSOR_XML_HANDLER_H

#include "service/SensorHistoryService.h"

#include "xmlui/SessionXmlHandler.h"

namespace BeeeOn {

namespace XmlUI {

class SensorXmlHandler : public SessionXmlHandler {
public:
	SensorXmlHandler(const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input,
			BeeeOn::Session::Ptr session,
			SensorHistoryService &sensorService);
	void handleInputImpl() override;

	void handleGetLog(const std::string &gateid,
			Poco::XML::Element *logsNode);

private:
	SensorHistoryService &m_sensorService;
};

class SensorXmlHandlerResolver : public SessionXmlHandlerResolver {
public:
	SensorXmlHandlerResolver();

	bool canHandle(const Poco::XML::Element &root);
	XmlRequestHandler *createHandler(
			const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input);

	void setSensorHistoryService(SensorHistoryService::Ptr service)
	{
		m_sensorService = service;
	}

	void setSessionManager(BeeeOn::SessionManager *manager)
	{
		m_sessionManager = manager;
	}

private:
	SensorHistoryService::Ptr m_sensorService;
	BeeeOn::SessionManager *m_sessionManager;
};

}
}

#endif

