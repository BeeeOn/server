#ifndef BEEEON_REST_HANDLER_H
#define BEEEON_REST_HANDLER_H

#include <Poco/Logger.h>

#include "Debug.h"

namespace BeeeOn {

class RestHandler {
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
			response.sendBuffer(data.c_str(), data.size());
			sendSuccess(response);
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

protected:
	Poco::Logger &m_logger;
};

}

#endif
