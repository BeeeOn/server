#ifndef BEEEON_ENUM_INFO_PROVIDER_H
#define BEEEON_ENUM_INFO_PROVIDER_H

#include <set>

#include "model/SubtypeInfo.h"
#include "provider/InfoProvider.h"
#include "util/EnumsSAXHandler.h"

namespace BeeeOn {

class EnumInfoProvider : public XmlInfoProvider<SubtypeInfo, EnumsSAXHandler> {
public:
	EnumInfoProvider();

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

