#include <Poco/Exception.h>
#include <Poco/SingletonHolder.h>
#include <Poco/Ascii.h>
#include <Poco/String.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/SAX/Locator.h>
#include <Poco/SAX/Attributes.h>
#include <Poco/SAX/SAXParser.h>
#include <Poco/SAX/InputSource.h>
#include <Poco/XML/XMLException.h>

#include "util/SAXHelper.h"
#include "Debug.h"

using namespace std;
using namespace Poco;
using namespace Poco::XML;
using namespace BeeeOn;

namespace BeeeOn {

class NoLocator : public Locator {
public:
	~NoLocator()
	{
	}

	XMLString getPublicId() const
	{
		return "<unknown>";
	}

	XMLString getSystemId() const
	{
		return "<unknown>";
	}

	int getLineNumber() const
	{
		return -1;
	}

	int getColumnNumber() const
	{
		return -1;
	}

	static Locator &instance();
};

}

Locator &NoLocator::instance()
{
	SingletonHolder<NoLocator> singleton;
	return *singleton.get();
}

SAXHelper::SAXHelper():
	m_locator(&NoLocator::instance())
{
}

SAXElement::SAXElement():
	localName(""),
	qName(""),
	uri("")
{
}

SAXElement::SAXElement(
		const Poco::XML::XMLString &localName,
		const Poco::XML::XMLString &qName,
		const Poco::XML::XMLString &uri):
	localName(localName),
	qName(qName),
	uri(uri)
{
}

bool SAXElement::isRoot() const
{
	return qName == "" && localName == "" && uri == "";
}

bool SAXElement::is(const Poco::XML::XMLString &_localName,
	const Poco::XML::XMLString &_qName,
	const Poco::XML::XMLString &_uri) const
{
	return _localName == localName
		&& _qName == qName
		&& _uri == uri;
}

SAXHelper::~SAXHelper()
{
}

void SAXHelper::processingInstruction(
		const XMLString &target, const XMLString &data)
{
	if (target == "warn") {
		string header(locator().getSystemId());
		header += " at line ";
		header += to_string(locator().getLineNumber());
		header += ", column ";
		header += to_string(locator().getColumnNumber());
		header += ": ";
		logger().warning(header + data, __FILE__, __LINE__);
	}
}

void SAXHelper::skippedEntity(const XMLString &name)
{
}

void SAXHelper::setDocumentLocator(const Locator *loc)
{
	m_locator = loc == NULL? &NoLocator::instance() : loc;
}

const string SAXHelper::beginMessage() const
{
	string loc;

	loc += "at line ";
	loc += to_string(locator().getLineNumber());
	loc += ", column ";
	loc += to_string(locator().getColumnNumber());
	loc += " ";

	loc += " in element ";
	loc += m_path.back().qName;
	loc += ": ";

	return loc;
}

void SAXHelper::error(const std::string &message) const
{
	throw XMLException(beginMessage() + message);
}

void SAXHelper::fatal(const std::string &message) const
{
	throw IllegalStateException(beginMessage() + message);
}

const Locator &SAXHelper::locator() const
{
	return *m_locator;
}

const deque<SAXElement> &SAXHelper::path() const
{
	return m_path;
}

const SAXElement &SAXHelper::current() const
{
	if (m_path.empty())
		fatal("no elements in path");

	return m_path.back();
}

const SAXElement &SAXHelper::parent() const
{
	if (m_path.size() < 2)
		fatal("no parent exists, we are at root");

	return m_path[1];
}

void SAXHelper::startDocument()
{
	m_path.push_back(SAXElement());
}

void SAXHelper::endDocument()
{
	if (m_path.empty()) {
		fatal("path is empty but should not be, top: "
			+ m_path.back().qName);
	}

	if (!m_path.back().isRoot()) {
		fatal("expected last element to be empty but has: "
			+ m_path.back().qName);
	}

	m_path.pop_back();

	if (!m_path.empty()) {
		fatal("path contains garbage: "
			+ m_path.back().qName);
	}
}

void SAXHelper::startElement(const XMLString &uri,
		const XMLString &localName,
		const XMLString &qName,
		const Attributes &attrList)
{
	SAXElement element(localName, qName.empty()? localName : qName, uri);

	if (!expectElement(element))
		error("unexpected element: " + qName);

	m_path.push_back(element);

	startElement(m_path.back(), attrList);
}

bool SAXHelper::expectElement(const SAXElement &element) const
{
	return true;
}

void SAXHelper::startElement(const SAXElement &element,
		const Poco::XML::Attributes &attrList)
{
}

void SAXHelper::endElement(const XMLString &uri,
		const XMLString &localName,
		const XMLString &_qName)
{
	const XMLString &qName = _qName.empty()? localName : _qName;

	if (m_path.empty())
		fatal("no more elements expected");

	const SAXElement &element = m_path.back();

	if (element.isRoot())
		fatal("expected end of document but got: " + qName);

	if (!element.is(localName, qName, uri)) {
		fatal("expected end of element: "
			+ m_path.back().qName + " but got: " + qName);
	}

	endElement(element);

	m_path.pop_back();
}

void SAXHelper::endElement(const SAXElement &element)
{
}

void SAXHelper::ignorableWhitespace(
		const XMLChar c[], int start, int length)
{
}

void SAXHelper::characters(const XMLChar c[], int start, int length)
{
	int modifiedStart = start;
	int modifiedLength = length;
	preprocessContent(c, modifiedStart, modifiedLength);

	if (modifiedStart < start || modifiedStart >= start + length)
		fatal("illegal starte after content preprocessing");

	if (modifiedLength > length - (start - modifiedStart))
		fatal("illegal length after content preprocessing");

	if (!expectContent(modifiedLength))
		error("unexpected content of length " + to_string(modifiedLength));

	appendContent(c, modifiedStart, modifiedLength);
}

bool SAXHelper::expectContent(int length) const
{
	return true;
}

void SAXHelper::preprocessContent(const XMLChar c[],
			int &start, int &length)
{
}

static bool isWhitespace(const XMLChar c)
{
	if (Ascii::isSpace(c))
		return true;

	return false;
}

void SAXHelper::trimContent(const XMLChar c[],
			int &start, int &length)
{
	if (length > 0) {
		for (int i = length - 1; i >= 0; --i) {
			if (isWhitespace(c[start + i]))
				length -= 1;
			else
				break;
		}
	}

	for (int i = 0; i < length; ++i) {
		if (isWhitespace(c[start + i]))
			start += 1;
		else
			break;
	}
}

void SAXHelper::appendContent(const XMLChar c[],
		int start, int length)
{
	appendContent(string(c + start, length));
}

void SAXHelper::appendContent(const XMLString &s)
{
	if (s.size() == 0)
		return;

	if (m_path.empty())
		fatal("path is empty, content is unexpected here");

	if (m_path.back().isRoot())
		fatal("we are at root, content is unexpected here");

	m_path.back().content += s;
}

bool SAXHelper::getAndTrimAttribute(const Attributes &attrList,
		const XMLString &name,
		XMLString &value) const
{
	int index = attrList.getIndex("", name);
	if (index < 0)
		return false;

	value = attrList.getValue(index);
	trimInPlace(value);

	return true;
}

void SAXHelper::startPrefixMapping(
		const XMLString &prefix, const XMLString &uri)
{
}

void SAXHelper::endPrefixMapping(const XMLString &prefix)
{
}

void SAXHelper::notationDecl(const XMLString &name,
		const XMLString *publicId,
		const XMLString *systemId)
{
}

void SAXHelper::unparsedEntityDecl(const XMLString &name,
		const XMLString *publicId,
		const XMLString &systemId,
		const XMLString &notationName)
{
}

void SAXHelper::parse(const Path &path, SAXHelper &helper)
{
	InputSource source(URI(path).toString());
	SAXParser parser;
	parser.setContentHandler(&helper);

	try {
		parser.parse(&source);
	}
	catch (const Exception &e) {
		LOGGER_FUNC(__func__).log(e, __FILE__, __LINE__);
		e.rethrow();
	}
}
