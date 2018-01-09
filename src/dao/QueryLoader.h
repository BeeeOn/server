#pragma once

#include <string>

namespace BeeeOn {

class QueryLoader {
public:
	QueryLoader();
	virtual ~QueryLoader();

	virtual std::string find(const std::string &key) const = 0;
};

}
