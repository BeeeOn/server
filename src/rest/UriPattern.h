#pragma once

#include <map>
#include <string>
#include <vector>

#include <Poco/URI.h>

#include "util/Loggable.h"

namespace BeeeOn {

class UriPattern : Loggable {
public:
	typedef std::map<std::string, std::string> Params;
	enum {
		PARAM_PREFIX = '$'
	};

	UriPattern(const std::string &pattern,
		const std::vector<std::string> &params);

	bool match(Poco::URI uri, Params &params) const;

	Poco::URI format(const std::vector<std::string> &params) const;

private:
	std::string m_pattern;
	std::vector<std::string> m_segments;
};

}
