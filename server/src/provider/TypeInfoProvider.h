#ifndef BEEEON_TYPE_INFO_PROVIDER_H
#define BEEEON_TYPE_INFO_PROVIDER_H

#include <set>

#include "model/TypeInfo.h"
#include "provider/InfoProvider.h"
#include "util/TypesSAXHandler.h"

namespace BeeeOn {

class TypeInfoProvider : public XmlInfoProvider<TypeInfo, TypesSAXHandler> {
public:
	TypeInfoProvider();

	void setTypesFile(const std::string &typesFile);

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
	std::string m_typesFile;
};

}

#endif
