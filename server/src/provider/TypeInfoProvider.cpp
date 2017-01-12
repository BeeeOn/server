#include "provider/TypeInfoProvider.h"

using namespace std;
using namespace Poco;
using namespace Poco::XML;
using namespace BeeeOn;

BEEEON_OBJECT(TypeInfoProvider, BeeeOn::TypeInfoProvider)

TypeInfoProvider::TypeInfoProvider()
{
	textInjector("typesFile", (TextSetter) &TypeInfoProvider::setTypesFile);
}

void TypeInfoProvider::setTypesFile(const std::string &typesFile)
{
	m_typesFile = typesFile;
}

void TypeInfoProvider::injectionDone()
{
	parseFile(m_typesFile, "type");
}
