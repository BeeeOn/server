#ifndef BEEEON_XMLUI_ABSTRACT_XML_HANDLER_H
#define BEEEON_XMLUI_ABSTRACT_XML_HANDLER_H

#include <Poco/Logger.h>
#include <Poco/SAX/AttributesImpl.h>

#include "server/XmlRequestHandler.h"
#include "server/Session.h"
#include "server/SessionManager.h"

namespace BeeeOn {
namespace XmlUI {

class AbstractXmlHandler : public BeeeOn::XmlRequestHandler {
public:
	AbstractXmlHandler(
			const std::string &ns,
			const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input);
	AbstractXmlHandler(
			const std::string &ns,
			const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input,
			BeeeOn::ExpirableSession::Ptr session);

	virtual void handleInput();
	virtual void handleInputImpl() = 0;

	bool requireSession();

	bool hasSession() const
	{
		return !m_session.isNull();
	}

	const BeeeOn::ExpirableSession::Ptr session() const
	{
		return m_session;
	}

	void resultSimple(Poco::XML::AttributesImpl &attrs,
			const std::string &result);
	void resultSuccess();
	void resultNotAuthenticated();
	void resultForbidden();
	void resultInvalidInput();
	void resultNotFound();
	void resultAlreadyAssigned();
	void resultNotOwned();
	void resultUnexpected();
	void resultError(const std::string &code);

	void resultDataStart();
	void resultDataEnd();

protected:
	Poco::Logger &m_logger;
	const std::string m_ns;
	BeeeOn::ExpirableSession::Ptr m_session;
};

class AbstractXmlHandlerResolver : public BeeeOn::XmlRequestHandlerResolver {
public:
	AbstractXmlHandlerResolver(
			const std::string &ns);

	virtual bool canHandle(const Poco::XML::Document &doc);
	virtual bool canHandle(const Poco::XML::Element &root);

protected:
	BeeeOn::ExpirableSession::Ptr lookupSession(
			BeeeOn::SessionManager &sessionManager,
			const Poco::AutoPtr<Poco::XML::Document> input);

protected:
	const std::string m_ns;
};

}
}

#endif
