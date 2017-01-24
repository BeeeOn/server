#ifndef BEEEON_XMLUI_DEVICE_XML_HANDLER_H
#define BEEEON_XMLUI_DEVICE_XML_HANDLER_H

#include "service/DeviceService.h"
#include "service/PlaceService.h"
#include "xmlui/AbstractXmlHandler.h"

namespace BeeeOn {
namespace XmlUI {

class DeviceXmlHandler : public AbstractXmlHandler {
public:
	DeviceXmlHandler(const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input,
			BeeeOn::ExpirableSession::Ptr session,
			BeeeOn::DeviceService &deviceService,
			BeeeOn::PlaceService &placeService);
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
	PlaceService &m_placeService;
};

class DeviceXmlHandlerResolver : public AbstractXmlHandlerResolver {
public:
	DeviceXmlHandlerResolver();

	bool canHandle(const Poco::XML::Element &root);
	XmlRequestHandler *createHandler(
			const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input);

	void setPlaceService(BeeeOn::PlaceService *service)
	{
		m_placeService = service;
	}

	void setDeviceService(BeeeOn::DeviceService *service)
	{
		m_deviceService = service;
	}

	void setSessionManager(BeeeOn::SessionManager *manager)
	{
		m_sessionManager = manager;
	}

private:
	BeeeOn::PlaceService *m_placeService;
	BeeeOn::DeviceService *m_deviceService;
	BeeeOn::SessionManager *m_sessionManager;
};

}
}

#endif
