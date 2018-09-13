#pragma once

#include <list>

#include <Poco/SharedPtr.h>

#include "server/HTTPFilter.h"

namespace BeeeOn {

class HTTPFilterChain {
public:
	typedef Poco::SharedPtr<HTTPFilterChain> Ptr;

	HTTPFilterChain();
	virtual ~HTTPFilterChain();

	void applyChain(
		Poco::Net::HTTPServerRequest &req,
		Poco::Net::HTTPServerResponse &res);

	void registerFilter(HTTPFilter::Ptr filter);

private:
	std::list<HTTPFilter::Ptr> m_chain;
};

}
