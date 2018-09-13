#pragma once

#include <map>
#include <string>
#include <vector>

#include <Poco/URI.h>

namespace BeeeOn {

class RestLinker {
public:
	virtual ~RestLinker();

	virtual std::string applicationName() const = 0;

	virtual Poco::URI link(
		const std::string &group,
		const std::string &action,
		const std::map<std::string, std::string> &queryString,
		const std::vector<std::string> &params) = 0;

	 Poco::URI link(
		const std::string &group,
		const std::string &action,
		const std::vector<std::string> &params = {})
	 {
		std::map<std::string, std::string> query;
		return link(group, action, query, params);
	 }
};

}
