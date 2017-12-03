#include "di/Injectable.h"
#include "xmlui/AnyXmlHandler.h"

using namespace std;
using namespace BeeeOn;
using namespace BeeeOn::XmlUI;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::XML;

AnyXmlHandler::AnyXmlHandler(
		const StreamSocket &socket,
		const AutoPtr<Document> input):
	AbstractXmlHandler("", socket, input)
{
}

void AnyXmlHandler::handleInputImpl()
{
	resultInvalidInput();
}

AnyXmlHandlerResolver::AnyXmlHandlerResolver():
	AbstractXmlHandlerResolver("")
{
}

bool AnyXmlHandlerResolver::canHandle(const Element &root)
{
	return true;
}

XmlRequestHandler *AnyXmlHandlerResolver::createHandler(
		const StreamSocket &socket,
		const AutoPtr<Document> input)
{
	return new AnyXmlHandler(socket, input);
}

BEEEON_OBJECT_BEGIN(BeeeOn, XmlUI, AnyXmlHandlerResolver)
BEEEON_OBJECT_CASTABLE(AbstractXmlHandlerResolver)
BEEEON_OBJECT_CASTABLE(XmlRequestHandlerResolver)
BEEEON_OBJECT_END(BeeeOn, XmlUI, AnyXmlHandlerResolver)
