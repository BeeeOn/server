#pragma once

#include <Poco/Timespan.h>

#include "l10n/Translator.h"
#include "rest/JSONRestHandler.h"
#include "service/ControlService.h"

namespace BeeeOn {

class Control;

namespace RestUI {

class ControlRestHandler : public JSONRestHandler {
public:
	ControlRestHandler();

	void setControlService(ControlService::Ptr service);
	void setTranslatorFactory(TranslatorFactory::Ptr factory);
	void setRequestTimeout(const Poco::Timespan &timeout);

	void list(RestFlow &flow);
	void detail(RestFlow &flow);
	void current(RestFlow &flow);
	void requestChange(RestFlow &flow);

protected:
	void prepareDetail(RestFlow &flow, Control &control);

private:
	ControlService::Ptr m_controlService;
	TranslatorFactory::Ptr m_translatorFactory;
	Poco::Timespan m_requestTimeout;
};

}
}
