#ifndef BEEEON_UTIL_SECURE_XML_PARSER_H
#define BEEEON_UTIL_SECURE_XML_PARSER_H

#include <Poco/SAX/SAXParser.h>
#include <Poco/XML/NamePool.h>
#include <Poco/DOM/Document.h>

namespace BeeeOn {

/**
 * The parser prevents parsing documents containing DTD definitions.
 * The purpose is to protect the system against XML attacks like:
 *
 * * Generic Entity Expansion
 * * Recursive Entity Expansion
 * * XML Remote Entity Expansion
 *
 * The parser throws InvalidArgumentException when a DTD section
 * is encountered.
 *
 * Parsing is thread-safe as for each parsing step a new parser
 * is allocated.
 */
class SecureXmlParser {
public:
	SecureXmlParser();
	~SecureXmlParser();

	Poco::XML::Document *parse(const char *input, std::size_t length);
	Poco::XML::Document *parse(const std::string &input);
};

}

#endif
