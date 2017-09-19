#ifndef BEEEON_TYPES_SAX_HANDLER_H
#define BEEEON_TYPES_SAX_HANDLER_H

#include <set>

#include "model/TypeInfo.h"
#include "util/SAXHelper.h"

namespace BeeeOn {

class TypesSAXHandler : public SAXHelper {
public:
	TypesSAXHandler();
	~TypesSAXHandler();

	std::set<TypeInfo>::const_iterator begin() const
	{
		return m_result.begin();
	}

	std::set<TypeInfo>::const_iterator end() const
	{
		return m_result.end();
	}

protected:
	bool expectElement(const SAXElement &element) const override;
	bool expectContent(int length) const override;
	void preprocessContent(const Poco::XML::XMLChar c[],
			int &start, int &length) override;

	void startElement(const SAXElement &element,
			const Poco::XML::Attributes &attrList) override;
	void endElement(const SAXElement &element) override;

private:
	std::set<Poco::XML::XMLString> m_typeExpect;
	std::set<Poco::XML::XMLString> m_leafElements;
	std::set<TypeInfo> m_result;
	TypeInfo m_temp;
};

}

#endif
