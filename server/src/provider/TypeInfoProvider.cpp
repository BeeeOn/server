#include "di/Injectable.h"
#include "provider/TypeInfoProvider.h"

using namespace std;
using namespace Poco;
using namespace Poco::XML;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, TypeInfoProvider)
BEEEON_OBJECT_CASTABLE(InfoProvider<TypeInfo>)
BEEEON_OBJECT_TEXT("typesFile", &TypeInfoProvider::setTypesFile)
BEEEON_OBJECT_HOOK("done", &TypeInfoProvider::loadInfo)
BEEEON_OBJECT_END(BeeeOn, TypeInfoProvider)

TypeInfoProvider::TypeInfoProvider()
{
}

void TypeInfoProvider::setTypesFile(const std::string &typesFile)
{
	m_typesFile = typesFile;
}

void TypeInfoProvider::loadInfo()
{
	parseFile(m_typesFile, "type");
}
