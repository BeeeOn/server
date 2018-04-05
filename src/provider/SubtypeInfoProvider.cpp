#include "di/Injectable.h"
#include "provider/SubtypeInfoProvider.h"
#include "util/EnumsSAXHandler.h"
#include "util/BitmapsSAXHandler.h"

using namespace std;
using namespace Poco;
using namespace Poco::XML;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, SubtypeInfoProvider)
BEEEON_OBJECT_CASTABLE(InfoProvider<SubtypeInfo>)
BEEEON_OBJECT_PROPERTY("enumsFile", &SubtypeInfoProvider::setEnumsFile)
BEEEON_OBJECT_PROPERTY("bitmapsFile", &SubtypeInfoProvider::setBitmapsFile)
BEEEON_OBJECT_HOOK("done", &SubtypeInfoProvider::loadInfo)
BEEEON_OBJECT_END(BeeeOn, SubtypeInfoProvider)

SubtypeInfoProvider::SubtypeInfoProvider()
{
}

void SubtypeInfoProvider::setEnumsFile(const std::string &enumsFile)
{
	m_enumsFile = enumsFile;
}

void SubtypeInfoProvider::setBitmapsFile(const std::string &bitmapsFile)
{
	m_bitmapsFile = bitmapsFile;
}

void SubtypeInfoProvider::loadInfo()
{
	parseFile<EnumsSAXHandler>(m_enumsFile, "enum");
	parseFile<BitmapsSAXHandler>(m_bitmapsFile, "bitmap");
}
