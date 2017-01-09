#ifndef BEEEON_REST_HANDLER_H
#define BEEEON_REST_HANDLER_H

#include <Poco/Logger.h>

#include "util/Loggable.h"
#include "Debug.h"

namespace BeeeOn {

class RestHandler : public Loggable {
public:
	RestHandler();

	template <typename Context>
	const std::string param(Context &context, const std::string &key)
	{
		return context.params().at(key);
	}

	template <typename Response>
	void sendResult(Response &response, const std::string &data)
	{
		if (logger().debug())
			logger().debug(data.c_str(), __FILE__, __LINE__);

		response.sendBuffer(data.c_str(), data.size());
		response.setStatusAndReason(
			Response::HTTP_OK);
	}

	template <typename Response>
	void sendResultOrNotFound(Response &response,
			const std::string &data)
	{
		if (data.empty()) {
			sendNotFound(response);
		}
		else {
			sendResult(response, data);
		}
	}

	template <typename Response>
	static void sendSuccess(Response &response)
	{
		TRACE_FUNC();

		response.setStatusAndReason(
			Response::HTTP_OK);
	}

	template <typename Response>
	static void sendUnauthorized(Response &response)
	{
		TRACE_FUNC();

		response.setStatusAndReason(
			Response::HTTP_UNAUTHORIZED);
	}

	template <typename Response>
	static void sendNoRoute(Response &response)
	{
		TRACE_FUNC();

		response.setStatusAndReason(
			Response::HTTP_NOT_FOUND);
	}

	template <typename Response>
	static void sendNotFound(Response &response)
	{
		TRACE_FUNC();

		response.setStatusAndReason(
			Response::HTTP_NOT_FOUND);
	}

	template <typename Response>
	static void sendNoOperation(Response &response)
	{
		TRACE_FUNC();

		response.setStatusAndReason(
			Response::HTTP_BAD_REQUEST);
	}

	template <typename Response>
	static void sendInvalidInput(Response &response)
	{
		TRACE_FUNC();

		response.setStatusAndReason(
			Response::HTTP_BAD_REQUEST);
	}
};

}

#endif
