#pragma once

#include <map>

#include <Poco/SharedPtr.h>

#include "rest/RestAction.h"
#include "rest/UriPattern.h"

namespace BeeeOn {

class MappedRestAction : public RestAction {
public:
	typedef Poco::SharedPtr<MappedRestAction> Ptr;
	typedef std::map<std::string, std::string> Params;

	MappedRestAction(
		const std::string &handlerName,
		const std::string &actionName,
		const UriPattern &pattern);

	void setCaching(int seconds);
	int caching() const;

	Poco::URI uri(const std::vector<std::string> &params = {}) const;

	bool match(const Poco::URI &uri, Params &params) const;

	static int parseCaching(const std::string &caching);

private:
	UriPattern m_uriPattern;
	int m_caching;
};

}
