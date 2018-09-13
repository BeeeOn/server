#pragma once

#include <Poco/Timespan.h>

#include "service/ControlService.h"
#include "service/SensorHistoryService.h"

#include "xmlui/SessionXmlHandler.h"

namespace BeeeOn {

namespace XmlUI {

class SensorXmlHandler : public SessionXmlHandler {
public:
	SensorXmlHandler(const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input,
			BeeeOn::Session::Ptr session,
			ControlService &controlService,
			SensorHistoryService &sensorService,
			const Poco::Timespan &setStateTimeout);
	void handleInputImpl() override;

	void handleGetLog(const std::string &gateid,
			Poco::XML::Element *logsNode);
	void handleSetState(const std::string &gateid,
			Poco::XML::Element *deviceNode);

private:
	ControlService &m_controlService;
	SensorHistoryService &m_sensorService;
	Poco::Timespan m_setStateTimeout;
};

class SensorXmlHandlerResolver : public SessionXmlHandlerResolver {
public:
	SensorXmlHandlerResolver();

	bool canHandle(const Poco::XML::Element &root);
	XmlRequestHandler *createHandler(
			const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input);

	void setControlService(ControlService::Ptr service);
	void setSensorHistoryService(SensorHistoryService::Ptr service);
	void setSetStateTimeout(const Poco::Timespan &timeout);

	void setSessionManager(BeeeOn::SessionManager *manager)
	{
		m_sessionManager = manager;
	}

private:
	ControlService::Ptr m_controlService;
	SensorHistoryService::Ptr m_sensorService;
	BeeeOn::SessionManager *m_sessionManager;
	Poco::Timespan m_setStateTimeout;
};

}
}
