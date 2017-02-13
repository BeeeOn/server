#include "provider/TypeInfoProvider.h"

using namespace std;
using namespace Poco;
using namespace Poco::XML;
using namespace BeeeOn;

BEEEON_OBJECT(BeeeOn, TypeInfoProvider)

TypeInfoProvider::TypeInfoProvider()
{
	textInjector("typesFile", &TypeInfoProvider::setTypesFile);
	hook("done", &TypeInfoProvider::loadInfo);
}

void TypeInfoProvider::setTypesFile(const std::string &typesFile)
{
	m_typesFile = typesFile;
}

void TypeInfoProvider::loadInfo()
{
	parseFile(m_typesFile, "type");
}
