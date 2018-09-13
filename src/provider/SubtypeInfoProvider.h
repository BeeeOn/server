#pragma once

#include <set>

#include "model/SubtypeInfo.h"
#include "provider/InfoProvider.h"

namespace BeeeOn {

class SubtypeInfoProvider : public XmlInfoProvider<SubtypeInfo> {
public:
	SubtypeInfoProvider();

	void setEnumsFile(const std::string &enumsFile);
	void setBitmapsFile(const std::string &bitmapsFile);

	InfoSet::const_iterator begin() const
	{
		return infoSet().begin();
	}

	InfoSet::const_iterator end() const
	{
		return infoSet().end();
	}

	void loadInfo();

private:
	std::string m_enumsFile;
	std::string m_bitmapsFile;
};

}
