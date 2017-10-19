#ifndef BEEEON_ENUMS_SAX_HANDLER_H
#define BEEEON_ENUMS_SAX_HANDLER_H

#include <set>

#include "model/EnumInfo.h"
#include "util/SAXHelper.h"

namespace BeeeOn {

class EnumsSAXHandler : public SAXHelper {
public:
	EnumsSAXHandler();
	~EnumsSAXHandler();

	std::set<EnumInfo>::const_iterator begin() const
	{
		return m_result.begin();
	}

	std::set<EnumInfo>::const_iterator end() const
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
	std::set<EnumInfo> m_result;
	int m_lastEquals;
	std::map<int, std::string> m_values;
	EnumInfo::Level m_level;
	std::set<EnumInfo::Level> m_levels;
	EnumInfo m_temp;
};

}

#endif

