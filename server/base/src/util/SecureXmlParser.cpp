#include <Poco/Exception.h>
#include <Poco/DOM/DOMBuilder.h>
#include <Poco/SAX/DeclHandler.h>
#include <Poco/SAX/WhitespaceFilter.h>
#include <Poco/SAX/LexicalHandler.h>

#include "util/SecureXmlParser.h"

using namespace std;
using namespace Poco;
using namespace Poco::XML;
using namespace BeeeOn;

namespace BeeeOn {

class DenyDTDHandler : public LexicalHandler {
public:
	void comment(const XMLChar c[], int start, int length)
	{
	}

	void startCDATA()
	{
	}

	void endCDATA()
	{
	}

	/**
	 * Throws InvalidArgumentException when called.
	 */
	void startDTD(
		const XMLString &name,
		const XMLString &publicId,
		const XMLString &systemId)
	{
		throw InvalidArgumentException(
				"DTD is forbidden for this parser");
	}

	void endDTD()
	{
	}

	void startEntity(const XMLString &name)
	{
	}

	void endEntity(const XMLString &name)
	{
	}
};

}

SecureXmlParser::SecureXmlParser()
{
}

SecureXmlParser::~SecureXmlParser()
{
}

Document *SecureXmlParser::parse(const char *input, std::size_t length)
{
	SAXParser parser;
	WhitespaceFilter filter(&parser);
	filter.setFeature(XMLReader::FEATURE_NAMESPACES, true);
	filter.setFeature(XMLReader::FEATURE_NAMESPACE_PREFIXES, true);

	DOMBuilder builder(filter);

	DenyDTDHandler lexicalHandler;
	filter.setProperty(XMLReader::PROPERTY_LEXICAL_HANDLER,
		static_cast<Poco::XML::LexicalHandler*>(&lexicalHandler));

	return builder.parseMemoryNP(input, length);
}

Document *SecureXmlParser::parse(const string &input)
{
	return parse(input.c_str(), input.size());
}
