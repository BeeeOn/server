#ifndef BEEEON_XMLUI_ROLE_XML_HANDLER_H
#define BEEEON_XMLUI_ROLE_XML_HANDLER_H

#include "xmlui/AbstractXmlHandler.h"

namespace BeeeOn {

class RoleService;
class SessionManager;

namespace XmlUI {

class RoleXmlHandler : public AbstractXmlHandler {
public:
	RoleXmlHandler(const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input,
			ExpirableSession::Ptr session,
			RoleService &roleService);

	void handleInputImpl() override;

protected:
	void handleInvite(const std::string &gateid,
			Poco::XML::Element *userNode);
	void handleGetAll(const std::string &gateid);
	void handleRemove(const std::string &gateid,
			Poco::XML::Element *userNode);
	void handleUpdateAccess(const std::string &gateid,
			Poco::XML::Element *userNode);

private:
	RoleService &m_roleService;
};

class RoleXmlHandlerResolver : public AbstractXmlHandlerResolver {
public:
	RoleXmlHandlerResolver();

	bool canHandle(const Poco::XML::Element &root);
	XmlRequestHandler *createHandler(
			const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input);

	void setRoleService(RoleService *service)
	{
		m_roleService = service;
	}

	void setSessionManager(BeeeOn::SessionManager *manager)
	{
		m_sessionManager = manager;
	}

private:
	RoleService *m_roleService;
	SessionManager *m_sessionManager;
};

}
}

#endif
