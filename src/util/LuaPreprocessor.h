#pragma once

#include <vector>

#include "util/Loggable.h"
#include "util/Preprocessor.h"

namespace BeeeOn {

/**
 * @brief Performs preprocessing by creating a Lua state and
 * evaluating the input.
 */
class LuaPreprocessor : public Preprocessor, Loggable {
public:
	/**
	 * @brief Process the given input. Chunks inside marks
	 * <code><?</code> and <code>?></code> are treated as
	 * Lua code and execute. The other chunks are treated
	 * as pure text and the intermediate Lua code would print
	 * it as is via a single statement.
	 */
	std::string process(const std::string &input) const override;

protected:
	struct Chunk {
		const std::string content;
		bool executable;
	};

	/**
	 * @brief Split the given input input into chunks of two kinds:
	 *
	 * <pre>
	 * <? echo("code"); ?>
	 * <? for i=1,10 do ?>
	 *   line <? echo(i); ?>
	 * <? end ?>
	 * </pre>
	 */
	std::vector<Chunk> splitChunks(const std::string &input) const;

	/**
	 * @brief Parse either a single textual chunk or a pair textual + executable
	 * chunks and return the current position in the input buffer.
	 */
	size_t parseChunks(
		const std::string &input,
		size_t at,
		std::vector<Chunk> &chunks) const;
};

}
