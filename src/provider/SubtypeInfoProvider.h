#ifndef BEEEON_SUBTYPE_INFO_PROVIDER_H
#define BEEEON_SUBTYPE_INFO_PROVIDER_H

#include <set>

#include "model/SubtypeInfo.h"
#include "provider/InfoProvider.h"

namespace BeeeOn {

class SubtypeInfoProvider : public XmlInfoProvider<SubtypeInfo> {
public:
	SubtypeInfoProvider();

	void setEnumsFile(const std::string &enumsFile);

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
};

}

#endif

