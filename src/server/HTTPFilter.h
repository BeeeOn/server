#pragma once

#include <Poco/SharedPtr.h>

namespace Poco {
namespace Net {

class HTTPServerRequest;
class HTTPServerResponse;

}
}

namespace BeeeOn {

/**
 * @brief Filter HTTP requests. It can inject HTTP headers, handle requests
 * (and thus avoiding to call the proper handler), precheck some HTTP headers, etc.
 */
class HTTPFilter {
public:
	typedef Poco::SharedPtr<HTTPFilter> Ptr;

	virtual ~HTTPFilter();

	/**
	 * Called on request before it is handled. The filter can handle
	 * the request and generate a response (it that case, it must flush
	 * response headers) prior to the request handler. The request handler
	 * is not called in such case.
	 */
	virtual void apply(
		Poco::Net::HTTPServerRequest &req,
		Poco::Net::HTTPServerResponse &res) = 0;
};

}
