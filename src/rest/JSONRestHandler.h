#pragma once

#include <exception>

#include <Poco/JSON/Object.h>

#include "rest/RestHandler.h"

namespace Poco {

class Exception;

namespace JSON {

class PrintHandler;

}
}

namespace BeeeOn {

class RestFlow;

class JSONRestHandler : public RestHandler {
public:
	JSONRestHandler(const std::string &name);

protected:
	Handler wrapHandler(const Handler &handler) override;

	Poco::JSON::Object::Ptr parseInput(RestFlow &flow) const;

	void handleException(
			RestFlow &flow,
			const Poco::Exception &e,
			const char *file,
			int line) const;
	void handleException(
			RestFlow &flow,
			const std::exception &e,
			const char *file,
			int line) const;

	void beginError(Poco::JSON::PrintHandler &result,
			unsigned int httpStatus,
			const std::string &status);
	void endError(Poco::JSON::PrintHandler &result);

	void beginSuccess(Poco::JSON::PrintHandler &result,
			unsigned int httpStatus = 200);
	void endSuccess(Poco::JSON::PrintHandler &result);
};

}
