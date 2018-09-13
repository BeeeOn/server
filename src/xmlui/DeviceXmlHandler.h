#pragma once

#include "service/DeviceService.h"
#include "xmlui/SessionXmlHandler.h"

namespace BeeeOn {

class CryptoConfig;

namespace XmlUI {

class DeviceXmlHandler : public SessionXmlHandler {
public:
	DeviceXmlHandler(const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input,
			BeeeOn::Session::Ptr session,
			BeeeOn::DeviceService &deviceService,
			BeeeOn::CryptoConfig *config);
	void handleInputImpl() override;

	void handleCreateParameter(const std::string &gateid,
			Poco::XML::Element *deviceNode);
	void handleUpdateParameter(const std::string &gateid,
			Poco::XML::Element *deviceNode);
	void handleDeleteParameter(const std::string &gateid,
			Poco::XML::Element *deviceNode);
	void handleGetParameter(const std::string &gateid,
			Poco::XML::Element *deviceNode);
	void handleUnregister(const std::string &gateid,
			Poco::XML::Element *deviceNode);
	void handleUpdate(const std::string &gateid,
			Poco::XML::Element *deviceNode);
	void handleGet(Poco::AutoPtr<Poco::XML::NodeList> deviceNode);
	void handleGetAll(const std::string &gateid);
	void handleGetNew(const std::string &gateid);

private:
	DeviceService &m_deviceService;
	CryptoConfig *m_config;
};

class DeviceXmlHandlerResolver : public SessionXmlHandlerResolver {
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

	void setCryptoConfig(CryptoConfig *config)
	{
		m_config = config;
	}

private:
	BeeeOn::CryptoConfig *m_config;
	BeeeOn::DeviceService *m_deviceService;
	BeeeOn::SessionManager *m_sessionManager;
};

}
}
