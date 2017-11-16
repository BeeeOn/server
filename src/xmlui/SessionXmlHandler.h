#ifndef BEEEON_XMLUI_SESSION_XML_HANDLER_H
#define BEEEON_XMLUI_SESSION_XML_HANDLER_H

#include "server/Session.h"
#include "server/SessionManager.h"
#include "xmlui/AbstractXmlHandler.h"

namespace BeeeOn {
namespace XmlUI {

class SessionXmlHandler : public AbstractXmlHandler {
public:
	SessionXmlHandler(
		const std::string &ns,
		const Poco::Net::StreamSocket &socket,
		const Poco::AutoPtr<Poco::XML::Document> input,
		Session::Ptr session);

	bool hasSession() const;
	bool requireSession();
	const Session::Ptr session() const;

private:
	Session::Ptr m_session;
};

class SessionXmlHandlerResolver : public AbstractXmlHandlerResolver {
public:
	SessionXmlHandlerResolver(const std::string &ns);

protected:
	Session::Ptr lookupSession(
			BeeeOn::SessionManager &sessionManager,
			const Poco::AutoPtr<Poco::XML::Document> input);
};

}
}

#endif
