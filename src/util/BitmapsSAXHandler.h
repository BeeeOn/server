#pragma once

#include <set>

#include "model/SubtypeInfo.h"
#include "util/SAXHelper.h"

namespace BeeeOn {

class BitmapsSAXHandler : public SAXHelper {
public:
	BitmapsSAXHandler();

	std::set<SubtypeInfo>::const_iterator begin() const
	{
		return m_result.begin();
	}

	std::set<SubtypeInfo>::const_iterator end() const
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
	std::set<SubtypeInfo> m_result;
	SubtypeInfoID m_id;
	int m_flagIndex;
	bool m_flagInversed;
	TypeInfo::Level::Attention m_flagAttention;
	TypeInfo::Level::Attention m_flagsDefaultAttention;
	BitmapInfo m_bitmap;
	BitmapInfo::Group m_group;
	std::map<uint8_t, BitmapInfo::Flag> m_flags;
	std::set<BitmapInfo::Group> m_groups;
};

}
