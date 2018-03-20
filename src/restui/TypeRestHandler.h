#ifndef BEEEON_SENSORTYPE_REST_HANDLER_H
#define BEEEON_SENSORTYPE_REST_HANDLER_H

#include "l10n/Translator.h"
#include "provider/SubtypeInfoProvider.h"
#include "provider/TypeInfoProvider.h"
#include "rest/JSONRestHandler.h"
#include "util/UnsafePtr.h"

namespace BeeeOn {
namespace RestUI {

class TypeRestHandler : public JSONRestHandler {
public:
	TypeRestHandler();

	void setTypeInfoProvider(TypeInfoProvider *provider);
	void setSubtypeInfoProvider(SubtypeInfoProvider *provider);
	void setTranslatorFactory(TranslatorFactory::Ptr factory);

	void list(RestFlow &flow);
	void detail(RestFlow &flow);
	void detailEnum(RestFlow &flow);

private:
	UnsafePtr<TypeInfoProvider> m_provider;
	UnsafePtr<SubtypeInfoProvider> m_subtypeProvider;
	TranslatorFactory::Ptr m_translatorFactory;
};

}
}

#endif
