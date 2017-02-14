#include "xmlui/AnyXmlHandler.h"
#include "Debug.h"

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
	_TRACE_METHOD(logger());
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

BEEEON_OBJECT(BeeeOn, XmlUI, AnyXmlHandlerResolver)
