#ifndef BEEEON_XMLUI_LOCATION_XML_HANDLER_H
#define BEEEON_XMLUI_LOCATION_XML_HANDLER_H

#include "service/LocationService.h"
#include "xmlui/AbstractXmlHandler.h"

namespace BeeeOn {
namespace XmlUI {

class LocationXmlHandler : public AbstractXmlHandler {
public:
	LocationXmlHandler(const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input,
			BeeeOn::ExpirableSession::Ptr session,
			BeeeOn::LocationService &locationService);
	void handleInputImpl() override;
	void handleCreate(const std::string &gateid,
			Poco::XML::Element *locationNode);
	void handleDelete(const std::string &gateid,
			Poco::XML::Element *locationNode);
	void handleUpdate(const std::string &gateid,
			Poco::XML::Element *locationNode);
	void handleGetAll(const std::string &gateid);

private:
	LocationService &m_locationService;
};

class LocationXmlHandlerResolver : public AbstractXmlHandlerResolver {
public:
	LocationXmlHandlerResolver();

	bool canHandle(const Poco::XML::Element &root);
	XmlRequestHandler *createHandler(
			const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input);

	void setLocationService(BeeeOn::LocationService *service)
	{
		m_locationService = service;
	}

	void setSessionManager(BeeeOn::SessionManager *manager)
	{
		m_sessionManager = manager;
	}

private:
	BeeeOn::LocationService *m_locationService;
	BeeeOn::SessionManager *m_sessionManager;
};

}
}

#endif

