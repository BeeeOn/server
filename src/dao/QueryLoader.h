#pragma once

#include <string>

#include <Poco/SharedPtr.h>

namespace BeeeOn {

class QueryLoader {
public:
	typedef Poco::SharedPtr<QueryLoader> Ptr;

	QueryLoader();
	virtual ~QueryLoader();

	virtual std::string find(const std::string &key) const = 0;
};

}
