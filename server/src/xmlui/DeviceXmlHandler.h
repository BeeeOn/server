#ifndef BEEEON_XMLUI_DEVICE_XML_HANDLER_H
#define BEEEON_XMLUI_DEVICE_XML_HANDLER_H

#include "service/DeviceService.h"
#include "xmlui/AbstractXmlHandler.h"

namespace BeeeOn {
namespace XmlUI {

class DeviceXmlHandler : public AbstractXmlHandler {
public:
	DeviceXmlHandler(const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input,
			BeeeOn::ExpirableSession::Ptr session,
			BeeeOn::DeviceService &deviceService);
	void handleInputImpl() override;

	void handleUnregister(const std::string &gateid,
			Poco::XML::Element *deviceNode);
	void handleUpdate(const std::string &gateid,
			Poco::XML::Element *deviceNode);
	void handleGet(Poco::AutoPtr<Poco::XML::NodeList> deviceNode);
	void handleGetAll(const std::string &gateid);
	void handleGetNew(const std::string &gateid);

private:
	DeviceService &m_deviceService;
};

class DeviceXmlHandlerResolver : public AbstractXmlHandlerResolver {
public:
	DeviceXmlHandlerResolver();

	bool canHandle(const Poco::XML::Element &root);
	XmlRequestHandler *createHandler(
			const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input);

	void setDeviceService(BeeeOn::DeviceService *service)
	{
		m_deviceService = service;
	}

	void setSessionManager(BeeeOn::SessionManager *manager)
	{
		m_sessionManager = manager;
	}

private:
	BeeeOn::DeviceService *m_deviceService;
	BeeeOn::SessionManager *m_sessionManager;
};

}
}

#endif
