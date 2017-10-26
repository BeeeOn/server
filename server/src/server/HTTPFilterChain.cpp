#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "di/Injectable.h"
#include "server/HTTPFilterChain.h"

BEEEON_OBJECT_BEGIN(BeeeOn, HTTPFilterChain)
BEEEON_OBJECT_REF("filters", &HTTPFilterChain::registerFilter)
BEEEON_OBJECT_END(BeeeOn, HTTPFilterChain)

using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

HTTPFilterChain::HTTPFilterChain()
{
}

HTTPFilterChain::~HTTPFilterChain()
{
}

void HTTPFilterChain::applyChain(
		HTTPServerRequest &req,
		HTTPServerResponse &res)
{
	for (auto filter : m_chain) {
		filter->apply(req, res);

		if (res.sent())
			return;
	}
}

void HTTPFilterChain::registerFilter(HTTPFilter::Ptr filter)
{
	m_chain.push_back(filter);
}
