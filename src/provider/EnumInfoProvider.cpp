#include "di/Injectable.h"
#include "provider/EnumInfoProvider.h"

using namespace std;
using namespace Poco;
using namespace Poco::XML;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, EnumInfoProvider)
BEEEON_OBJECT_CASTABLE(InfoProvider<EnumInfo>)
BEEEON_OBJECT_TEXT("enumsFile", &EnumInfoProvider::setEnumsFile)
BEEEON_OBJECT_HOOK("done", &EnumInfoProvider::loadInfo)
BEEEON_OBJECT_END(BeeeOn, EnumInfoProvider)

EnumInfoProvider::EnumInfoProvider()
{
}

void EnumInfoProvider::setEnumsFile(const std::string &enumsFile)
{
	m_enumsFile = enumsFile;
}

void EnumInfoProvider::loadInfo()
{
	parseFile(m_enumsFile, "enum");
}
