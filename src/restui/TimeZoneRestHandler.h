#pragma once

#include "l10n/TimeZoneProvider.h"
#include "rest/JSONRestHandler.h"

namespace BeeeOn {
namespace RestUI {

class TimeZoneRestHandler : public JSONRestHandler {
public:
	TimeZoneRestHandler();

	void setTimeZoneProvider(TimeZoneProvider::Ptr provider);

	void list(RestFlow &flow);
	void detailByID(RestFlow &flow);
	void detailByPair(RestFlow &flow);

protected:
	void detail(RestFlow &flow, const std::string &id);

private:
	TimeZoneProvider::Ptr m_provider;
};

}
}
