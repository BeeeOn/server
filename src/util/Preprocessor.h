#pragma once

#include <string>

#include <Poco/SharedPtr.h>

namespace BeeeOn {

/**
 * @brief Preprocessor of some textual input.
 */
class Preprocessor {
public:
	typedef Poco::SharedPtr<Preprocessor> Ptr;

	virtual ~Preprocessor();

	/**
	 * @brief Preprocess the given input and return the preprocessed result.
	 */
	virtual std::string process(const std::string &input) const = 0;
};

}
