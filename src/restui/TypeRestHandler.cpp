#include <Poco/Exception.h>
#include <Poco/JSON/PrintHandler.h>

#include "di/Injectable.h"
#include "model/User.h"
#include "model/Gateway.h"
#include "service/GatewayService.h"
#include "service/DeviceService.h"
#include "rest/RestFlow.h"
#include "rest/RestLinker.h"
#include "restui/TypeRestHandler.h"
#include "restui/JSONDeviceDeserializer.h"
#include "restui/Serializing.h"
#include "util/JsonUtil.h"

BEEEON_OBJECT_BEGIN(BeeeOn, RestUI, TypeRestHandler)
BEEEON_OBJECT_CASTABLE(RestHandler)
BEEEON_OBJECT_REF("typeInfoProvider", &TypeRestHandler::setTypeInfoProvider)
BEEEON_OBJECT_REF("enumInfoProvider", &TypeRestHandler::setEnumInfoProvider)
BEEEON_OBJECT_END(BeeeOn, RestUI, TypeRestHandler)

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;
using namespace BeeeOn::RestUI;

BeeeOn::RestUI::TypeRestHandler::TypeRestHandler():
		JSONRestHandler("types")
{
	registerAction<TypeRestHandler>("list", &TypeRestHandler::list);
	registerAction<TypeRestHandler>("detail", &TypeRestHandler::detail, {"type_id"});
	registerAction<TypeRestHandler>("detail_enum", &TypeRestHandler::detailEnum, {"enum_id"});
}

void TypeRestHandler::setTypeInfoProvider(TypeInfoProvider *typeInfoProvider)
{
	m_provider = typeInfoProvider;
}

void TypeRestHandler::setEnumInfoProvider(EnumInfoProvider *enumInfoProvider)
{
	m_enumProvider = enumInfoProvider;
}

void TypeRestHandler::list(RestFlow &flow)
{
	PrintHandler result(flow.response().stream());

	beginSuccess(result, 200);
	serialize(result, *flow.translator(), m_provider->begin(), m_provider->end());
	endSuccess(result);
}

void TypeRestHandler::detail(RestFlow &flow)
{
	SimpleID id = SimpleID::parse(flow.param("type_id"));
	SharedPtr<TypeInfo> info = m_provider->findById(id);

	if (info.isNull())
		throw NotFoundException("no such type " + id.toString());

	PrintHandler result(flow.response().stream());

	beginSuccess(result, 200);
	serialize(result, *flow.translator(), *info);
	endSuccess(result);
}

void TypeRestHandler::detailEnum(RestFlow &flow)
{
	EnumInfoID id = EnumInfoID::parse(flow.param("enum_id"));
	SharedPtr<EnumInfo> info = m_enumProvider->findById(id);

	if (info.isNull())
		throw NotFoundException("no such enum " + id.toString());

	PrintHandler result(flow.response().stream());

	beginSuccess(result, 200);
	serialize(result, *flow.translator(), *info);
	endSuccess(result);
}
