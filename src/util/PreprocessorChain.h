#pragma once

#include <vector>

#include "util/Preprocessor.h"

namespace BeeeOn {

/**
 * @brief A chain of preprocessors to be called sequentially.
 * The output of the i'th preprocessor is used as an input for
 * the (i+1)'th preprocessor.
 */
class PreprocessorChain : public Preprocessor {
public:
	void add(Preprocessor::Ptr pp);

	/**
	 * @brief Preprocess the given input according to the
	 * installed preprocessor chain.
	 */
	std::string process(const std::string &input) const override;

private:
	std::vector<Preprocessor::Ptr> m_chain;
};

}
