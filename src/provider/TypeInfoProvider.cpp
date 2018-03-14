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

	for (const auto &type : ModuleType::Type::all()) {
		const TypeInfoID &id = {type};

		if (findById(id).isNull()) {
			TypeInfo info(id);
			info.setName(type.toString());
			info.setUnit(ModuleType(type).baseUnit().symbol());

			if (!registerInfo(info))
				continue;

			logger().information("register type "
				+ info.name()
				+ " with ID " + info.id().toString(),
				__FILE__, __LINE__);
		}
	}
}
