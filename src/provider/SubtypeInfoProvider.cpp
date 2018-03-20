#include "di/Injectable.h"
#include "provider/SubtypeInfoProvider.h"

using namespace std;
using namespace Poco;
using namespace Poco::XML;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, SubtypeInfoProvider)
BEEEON_OBJECT_CASTABLE(InfoProvider<SubtypeInfo>)
BEEEON_OBJECT_TEXT("enumsFile", &SubtypeInfoProvider::setEnumsFile)
BEEEON_OBJECT_HOOK("done", &SubtypeInfoProvider::loadInfo)
BEEEON_OBJECT_END(BeeeOn, SubtypeInfoProvider)

SubtypeInfoProvider::SubtypeInfoProvider()
{
}

void SubtypeInfoProvider::setEnumsFile(const std::string &enumsFile)
{
	m_enumsFile = enumsFile;
}

void SubtypeInfoProvider::loadInfo()
{
	parseFile(m_enumsFile, "enum");
}
