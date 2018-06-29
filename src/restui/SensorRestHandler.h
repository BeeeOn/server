#ifndef BEEEON_SENSOR_REST_HANDLER_H
#define BEEEON_SENSOR_REST_HANDLER_H

#include <Poco/Timespan.h>

#include "l10n/Translator.h"
#include "rest/JSONRestHandler.h"
#include "service/SensorService.h"
#include "service/SensorHistoryService.h"
#include "util/TimeInterval.h"

namespace BeeeOn {

class Sensor;

namespace RestUI {

class SensorRestHandler : public JSONRestHandler {
public:
	SensorRestHandler();

	void setSensorService(SensorService::Ptr service);
	void setSensorHistoryService(SensorHistoryService::Ptr service);
	void setTranslatorFactory(TranslatorFactory::Ptr factory);

	void list(RestFlow &flow);
	void detail(RestFlow &flow);
	void current(RestFlow &flow);
	void history(RestFlow &flow);

protected:
	void prepareDetail(RestFlow &flow, Sensor &sensor);

private:
	TimeInterval parseRange(const std::string &input) const;
	Poco::Timespan parseInterval(const std::string &input) const;
	std::vector<std::string> parseAggregation(const std::string &input) const;

private:
	SensorService::Ptr m_sensorService;
	SensorHistoryService::Ptr m_sensorHistoryService;
	TranslatorFactory::Ptr m_translatorFactory;
};

}
}

#endif
