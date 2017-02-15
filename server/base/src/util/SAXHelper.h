#ifndef BEEEON_SAX_HELPER_H
#define BEEEON_SAX_HELPER_H

#include <stack>

#include <Poco/SAX/ContentHandler.h>
#include <Poco/SAX/DTDHandler.h>

#include "util/Loggable.h"

namespace Poco {

class Path;

namespace XML {

class Locator;

}
}

namespace BeeeOn {

struct SAXElement {
	SAXElement();
	SAXElement(
		const Poco::XML::XMLString &localName,
		const Poco::XML::XMLString &qName,
		const Poco::XML::XMLString &uri
	);

	bool isRoot() const;
	bool is(const Poco::XML::XMLString &localName,
		const Poco::XML::XMLString &qName,
		const Poco::XML::XMLString &uri) const;

	const Poco::XML::XMLString localName;
	const Poco::XML::XMLString qName;
	const Poco::XML::XMLString uri;
	Poco::XML::XMLString content;
};

class SAXHelper :
		public Poco::XML::ContentHandler,
		public Poco::XML::DTDHandler,
		public Loggable {
public:
	SAXHelper();
	virtual ~SAXHelper();
	
	virtual void processingInstruction(
			const Poco::XML::XMLString &target,
			const Poco::XML::XMLString &data);

	virtual void skippedEntity(const Poco::XML::XMLString &name);

	/**
	 * Whitespace is ignored as default.
	 */
	virtual void ignorableWhitespace(
			const Poco::XML::XMLChar c[],
			int start,
			int length) override;

	/**
	 * The default implementation tests expectContent and if
	 * succeeds it calls simply appendContent. If no content
	 * is expected, it generates an error.
	 */
	virtual void characters(const Poco::XML::XMLChar c[],
			int start, int length) override;

	/**
	 * Inserts the root SAXElement into path.
	 */
	void startDocument() override;
	
	/**
	 * Expects the root SAXElement as the last one on the path.
	 */
	void endDocument() override;

	/**
	 * Check whether the incoming element is expected,
	 * push it on top of the path and calls user defined
	 * startElement().
	 */
	void startElement(const Poco::XML::XMLString &uri,
			const Poco::XML::XMLString &localName,
			const Poco::XML::XMLString &qName,
			const Poco::XML::Attributes &attrList) override;
	
	/**
	 * Check whether the closing element is expected,
	 * pops the one on top of the path and calls user defined
	 * endElement().
	 */
	void endElement(const Poco::XML::XMLString &uri,
			const Poco::XML::XMLString &localName,
			const Poco::XML::XMLString &qName) override;

	virtual void startPrefixMapping(
			const Poco::XML::XMLString &prefix,
			const Poco::XML::XMLString &uri) override;

	virtual void endPrefixMapping(const Poco::XML::XMLString &prefix) override;
	
	void setDocumentLocator(const Poco::XML::Locator *loc) override;

	virtual void notationDecl(const Poco::XML::XMLString &name,
			const Poco::XML::XMLString *publicId,
			const Poco::XML::XMLString *systemId) override;

	virtual void unparsedEntityDecl(const Poco::XML::XMLString &name,
			const Poco::XML::XMLString *publicId,
			const Poco::XML::XMLString &systemId,
			const Poco::XML::XMLString &notationName) override;

	static void parse(const Poco::Path &path, SAXHelper &helper);

protected:
	/**
	 * Return true when the given element is expected.
	 * This is up to the user to implement it.
	 */
	virtual bool expectElement(const SAXElement &element) const;

	/**
	 * Handle start-element event.
	 * This is up to the user to implement it.
	 */
	virtual void startElement(const SAXElement &element,
			const Poco::XML::Attributes &attrList);

	/**
	 * Handle end-element event.
	 * This is up to the user to implement it.
	 */
	virtual void endElement(const SAXElement &element);

	/**
	 * Return true when a content is expected for the current
	 * path and with the given length.
	 */
	virtual bool expectContent(int length) const;

	/**
	 * Allow to preprocess data by characters() call before
	 * passing to appendContent.
	 */
	virtual void preprocessContent(
			const Poco::XML::XMLChar c[],
			int &start,
			int &length);

	/**
	 * Trim whitespace of the given content.
	 */
	void trimContent(const Poco::XML::XMLChar c[],
			int &start,
			int &length);

	/**
	 * Apend characters into the buffer of the current element.
	 * The buffer is accessible as current().content.
	 */
	void appendContent(const Poco::XML::XMLChar c[],
			int start, int length);

	/**
	 * Apend characters into the buffer of the current element.
	 * The buffer is accessible as current().content.
	 */
	void appendContent(const Poco::XML::XMLString &s);

	/**
	 * Obtain trimmed attribute value by the given name.
	 * Returns false when no such attribute exists.
	 */
	bool getAndTrimAttribute(const Poco::XML::Attributes &attrList,
			const Poco::XML::XMLString &name,
			Poco::XML::XMLString &value) const;

	/**
	 * Throw exception with the given error message.
	 */
	void error(const std::string &message) const;

	/**
	 * Throw exception with the given fatal message.
	 */
	void fatal(const std::string &message) const;

	/**
	 * Return begin of an error or failure message.
	 */
	const std::string beginMessage() const;

	/**
	 * Access locator.
	 */
	const Poco::XML::Locator &locator() const;

	/**
	 * Stack of seen elements.
	 */
	const std::deque<SAXElement> &path() const;

	/**
	 * Current element being processed.
	 */
	const SAXElement &current() const;

	/**
	 * Access parent element.
	 */
	const SAXElement &parent() const;

	/**
	 * Test for XPath expression: /<name>
	 */
	template <typename T>
	bool isPathFromRoot(const T &name) const
	{
		return m_path.size() > 1
			&& m_path[0].isRoot()
			&& checkPath(1, name);
	}

	/**
	 * Test for XPath expression: /<name0>{/<name1>...}*
	 */
	template <typename T, typename ...Ts>
	bool isPathFromRoot(const T &name, const Ts &... ts) const
	{
		return m_path.size() > 1
			&& m_path[0].isRoot()
			&& checkPath(1, name, ts...);
	}

private:
	bool checkPath(unsigned int i) const
	{
		return i == m_path.size();
	}

	template <typename T>
	bool checkPath(unsigned int i, const T &name) const
	{
		return i < m_path.size()
			&& matchPathElement(m_path[i], name)
			&& checkPath(i + 1);
	}

	template <typename T, typename ...Ts>
	bool checkPath(unsigned int i, const T &name, const Ts &... ts) const
	{
		return i < m_path.size()
			&& matchPathElement(m_path[i], name)
			&& checkPath(i + 1, ts...);
	}

	template <typename T>
	bool matchPathElement(const SAXElement &element, const T &name) const
	{
		return element.localName == name || name == std::string("*");
	}

private:
	const Poco::XML::Locator *m_locator;
	std::deque<SAXElement> m_path;
};

}

#endif
