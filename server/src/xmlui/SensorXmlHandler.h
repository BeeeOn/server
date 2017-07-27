#ifndef BEEEON_XMLUI_SENSOR_XML_HANDLER_H
#define BEEEON_XMLUI_SENSOR_XML_HANDLER_H

#include "xmlui/AbstractXmlHandler.h"

namespace BeeeOn {

class SensorHistoryService;

namespace XmlUI {

class SensorXmlHandler : public AbstractXmlHandler {
public:
	SensorXmlHandler(const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input,
			BeeeOn::Session::Ptr session,
			BeeeOn::SensorHistoryService &sensorService);
	void handleInputImpl() override;

	void handleGetLog(const std::string &gateid,
			Poco::XML::Element *logsNode);

private:
	SensorHistoryService &m_sensorService;
};

class SensorXmlHandlerResolver : public AbstractXmlHandlerResolver {
public:
	SensorXmlHandlerResolver();

	bool canHandle(const Poco::XML::Element &root);
	XmlRequestHandler *createHandler(
			const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input);

	void setSensorHistoryService(BeeeOn::SensorHistoryService *service)
	{
		m_sensorService = service;
	}

	void setSessionManager(BeeeOn::SessionManager *manager)
	{
		m_sessionManager = manager;
	}

private:
	BeeeOn::SensorHistoryService *m_sensorService;
	BeeeOn::SessionManager *m_sessionManager;
};

}
}

#endif

