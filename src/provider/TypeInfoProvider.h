#ifndef BEEEON_TYPE_INFO_PROVIDER_H
#define BEEEON_TYPE_INFO_PROVIDER_H

#include <set>

#include "model/TypeInfo.h"
#include "provider/InfoProvider.h"

namespace BeeeOn {

class TypeInfoProvider : public XmlInfoProvider<TypeInfo> {
public:
	TypeInfoProvider();

	void setTypesFile(const std::string &typesFile);

	void loadInfo();

private:
	std::string m_typesFile;
};

}

#endif
