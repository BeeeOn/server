#ifndef BEEEON_REST_LINKER_H
#define BEEEON_REST_LINKER_H

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
		const std::vector<std::string> &params = {}) = 0;
};

}

#endif
