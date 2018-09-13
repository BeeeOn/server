#pragma once

#include "xmlui/AbstractXmlHandler.h"

namespace BeeeOn {
namespace XmlUI {

class AnyXmlHandler : public AbstractXmlHandler {
public:
	AnyXmlHandler(const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input);

	void handleInputImpl() override;
};

class AnyXmlHandlerResolver : public AbstractXmlHandlerResolver {
public:
	AnyXmlHandlerResolver();

	bool canHandle(const Poco::XML::Element &input);
	XmlRequestHandler *createHandler(
			const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input);
};

}
}
