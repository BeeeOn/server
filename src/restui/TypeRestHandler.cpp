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
BEEEON_OBJECT_PROPERTY("typeInfoProvider", &TypeRestHandler::setTypeInfoProvider)
BEEEON_OBJECT_PROPERTY("subtypeInfoProvider", &TypeRestHandler::setSubtypeInfoProvider)
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
	registerAction<TypeRestHandler>("detail_subtype", &TypeRestHandler::detailSubtype, {"type", "subtype_id"});
}

void TypeRestHandler::setTypeInfoProvider(TypeInfoProvider *typeInfoProvider)
{
	m_provider = typeInfoProvider;
}

void TypeRestHandler::setSubtypeInfoProvider(SubtypeInfoProvider *subtypeInfoProvider)
{
	m_subtypeProvider = subtypeInfoProvider;
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
	TypeInfoID id = TypeInfoID::parse(flow.param("type_id"));
	SharedPtr<TypeInfo> info = m_provider->findById(id);

	if (info.isNull())
		throw NotFoundException("no such type " + id.toString());

	PrintHandler result(flow.response().stream());

	beginSuccess(result, 200);
	serialize(result, *flow.translator(), *info);
	endSuccess(result);
}

void TypeRestHandler::detailSubtype(RestFlow &flow)
{
	SubtypeInfo::Kind kind = SubtypeInfo::KIND_INVALID;
	if (flow.param("type") == "enum")
		kind = SubtypeInfo::KIND_ENUM;
	else if (flow.param("type") == "bitmap")
		kind = SubtypeInfo::KIND_BITMAP;

	SubtypeInfoID id = SubtypeInfoID::parse(flow.param("subtype_id"));
	SharedPtr<SubtypeInfo> info = m_subtypeProvider->findById(id);

	if (info.isNull())
		throw NotFoundException("no such subtype " + id.toString());

	if (info->kind() != kind)
		throw NotFoundException("subtype is of different kind");

	PrintHandler result(flow.response().stream());

	beginSuccess(result, 200);
	serialize(result, *flow.translator(), *info);
	endSuccess(result);
}
