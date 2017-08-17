#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/Net/NetException.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/PrintHandler.h>

#include "rest/JSONRestHandler.h"
#include "rest/RestFlow.h"
#include "rest/RestLinker.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::JSON;
using namespace BeeeOn;

static const string STATUS_SUCCESS = "success";
static const string STATUS_ERROR   = "error";
static const string STATUS_FAIL    = "fail";

JSONRestHandler::JSONRestHandler(const std::string &name):
	RestHandler(name)
{
}

void JSONRestHandler::handleException(
		RestFlow &flow,
		const Exception &e,
		const char *file,
		int line) const
{
	logger().log(e, file, line);

	if (flow.response().impl().sent())
		e.rethrow();
}

void JSONRestHandler::handleException(
		RestFlow &flow,
		const exception &e,
		const char *file,
		int line) const
{
	logger().critical(e.what(), file, line);

	if (flow.response().impl().sent())
		throw e;
}

RestHandler::Handler JSONRestHandler::wrapHandler(const Handler &handler)
{
	Handler wrapped = [this, handler](RestFlow &flow) {
		const PocoRequest &request = flow.request();
		PocoResponse &response = flow.response();

		response.impl().setVersion(HTTPResponse::HTTP_1_1);

		string message;
		string status = "fail";

		try {
			if (!request.accepts("application", "json"))
				throw InvalidArgumentException("client cannot handle type application/json");

			response.impl().set("Content-Type", "application/json; charset=utf-8");
			handler(flow);

			if (logger().debug()) {
				logger().debug("result of "
					+ flow.uri().toString()
					+ ": "
					+ to_string(response.status()),
					__FILE__, __LINE__);
			}

			if (!response.impl().sent())
				response.impl().send();

			return;
		}
		catch (const NotAuthenticatedException &e) {
			handleException(flow, e, __FILE__, __LINE__);

			response.impl().requireAuthentication(
				flow.linker().applicationName()	
			);

			status = "error";
			message = "not authenticated";
		}
		catch (const NotFoundException &e) {
			handleException(flow, e, __FILE__, __LINE__);

			response.setStatus(HTTPResponse::HTTP_NOT_FOUND);
			status = "error";
			message = "requested resource doest not exist";
		}
		catch (const InvalidAccessException &e) {
			handleException(flow, e, __FILE__, __LINE__);

			response.setStatus(HTTPResponse::HTTP_FORBIDDEN);
			status = "error";
			message = "not enough permission to access the resource";
		}
		catch (const SyntaxException &e) {
			handleException(flow, e, __FILE__, __LINE__);

			response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
			status = "error";
			message = "cannot process input data";
		}
		catch (const InvalidArgumentException &e) {
			handleException(flow, e, __FILE__, __LINE__);

			response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
			status = "error";
			message = "malformed input data";
		}
		catch (const Exception &e) {
			handleException(flow, e, __FILE__, __LINE__);

			response.setStatus(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
			message = "internal error on the server";
		}
		catch (const exception &e) {
			handleException(flow, e, __FILE__, __LINE__);

			response.setStatus(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
			message = "internal error on the server";
		}

		PrintHandler result(response.impl().send());
		beginError(result, response.status(), status);
		result.value(message);
		endError(result);
	};

	return wrapped;
}

Object::Ptr JSONRestHandler::parseInput(RestFlow &flow) const
{
	Parser parser;
	auto result = parser.parse(flow.request().stream());
	return result.extract<Object::Ptr>();
}

void JSONRestHandler::beginError(
		PrintHandler &result,
		unsigned int httpStatus,
		const string &status)
{
	result.startObject();
	result.key("code");
	result.value(httpStatus);
	result.key("status");
	result.value(status);
	result.key("message");
}

void JSONRestHandler::endError(PrintHandler &result)
{
	result.endObject();
}

void JSONRestHandler::beginSuccess(PrintHandler &result, unsigned int httpStatus)
{
	result.startObject();
	result.key("code");
	result.value(httpStatus);
	result.key("status");
	result.value(STATUS_SUCCESS);
	result.key("data");
}

void JSONRestHandler::endSuccess(PrintHandler &result)
{
	result.endObject();
}
