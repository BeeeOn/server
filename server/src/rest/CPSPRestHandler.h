#ifndef BEEEON_CPSP_REST_HANDLER_H
#define BEEEON_CPSP_REST_HANDLER_H

#include <map>
#include <string>

#include <Poco/SharedPtr.h>
#include <Poco/Net/HTTPRequestHandler.h>

#include "rest/RestHandler.h"

namespace BeeeOn {

/**
 * @briefCPSPRestHandler is intended to server CPSP pages via the REST framework.
 * All webpages must be compiled into classes and registered with the CPSPRestHandler
 * instance. This can be done via the dependency injection. Each registered page must
 * be annotated via the BEEEON_OBJECT_* macros.
 *
 * The REST mapping to actions is done via the class name of the CPSP page. If a class
 * MyNS::IndexHandler is generated for a page, then the mapping is done as:
 *
 * <pre>
 * api.call.cpsp.MyNS::IndexHandler.uri = /myns/index
 * </pre>
 *
 * No arguments are possible. To support arguments, you must use URI wildcards
 * like <code>/myns/index/*</code> and parse the URI manually by the page.
 */
class CPSPRestHandler : public RestHandler {
public:
	CPSPRestHandler();

	void registerHandler(Poco::SharedPtr<Poco::Net::HTTPRequestHandler> handler);
};

}

#endif
