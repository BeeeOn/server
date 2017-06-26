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
BEEEON_OBJECT_REF("translatorFactory", &TypeRestHandler::setTranslatorFactory)
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
}

void TypeRestHandler::setTypeInfoProvider(TypeInfoProvider *typeInfoProvider)
{
	m_provider = typeInfoProvider;
}

void TypeRestHandler::setTranslatorFactory(TranslatorFactory::Ptr factory)
{
	m_translatorFactory = factory;
}

void TypeRestHandler::list(RestFlow &flow)
{
	Translator::Ptr translator =
		m_translatorFactory->create(flow.session()->locale());

	PrintHandler result(flow.response().stream());

	beginSuccess(result, 200);
	serialize(result, *translator, m_provider->begin(), m_provider->end());
	endSuccess(result);
}

void TypeRestHandler::detail(RestFlow &flow)
{
	Translator::Ptr translator =
		m_translatorFactory->create(flow.session()->locale());

	SimpleID id = SimpleID::parse(flow.param("type_id"));
	SharedPtr<TypeInfo> info = m_provider->findById(id);

	if (info.isNull())
		throw NotFoundException("no such type " + id.toString());

	PrintHandler result(flow.response().stream());

	beginSuccess(result, 200);
	serialize(result, *translator, *info);
	endSuccess(result);
}
