#include <sstream>

#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/SHA1Engine.h>

#include "util/LuaPreprocessor.h"
#include "util/LuaEngine.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

static string MARK_OPEN  = "<?";
static string MARK_CLOSE = "?>";

static Logger &luaLogger()
{
	return Loggable::forClass(typeid(LuaPreprocessor));
}

/**
 * @brief Implement special command with _echo_ semantics that accepts
 * a single argument that can be either string or a number. It prints
 * the argument into the associated ostream given via the light userdata
 * inside the function's closure.
 */
static int luaCommandEcho(lua_State *state)
{
	LuaEngine engine(state, false);

	void *p = engine.toUserData(LuaEngine::upvalueIndex(1));
	ostream &out = *static_cast<ostream *>(p);

	out << engine.toString(1);
	return 1;
}

/**
 * @brief Define function echo having the ostream inside its closure.
 */
static void luaDefineEcho(LuaEngine &engine, ostream &out)
{
	engine.pushLightUserData(reinterpret_cast<void *>(&out));
	engine.pushCClosure(&luaCommandEcho, 1);
	engine.setGlobal("echo");
}

/**
 * @brief Implement a custom command that prints an associated text
 * into an associated ostream. The text to be printed and the ostream
 * are both parts of the function's closure.
 */
static int luaCommandEchoConstant(lua_State *state)
{
	LuaEngine engine(state, false);

	void *p = engine.toUserData(LuaEngine::upvalueIndex(1));
	ostream &out = *static_cast<ostream *>(p);

	out << engine.toString(LuaEngine::upvalueIndex(2));
	return 0;
}

/**
 * @brief Construct a custom command that just prints the given argument _input_
 * into the given ostream. Empty strings are ignored entirely. The method returns
 * a Lua piece of code (a single statement) that invokes the actual print.
 * It has form of a single function call. The called function is generated and its
 * name is derived from SHA-1 digest of the string to be printed.
 */
static string luaEchoConstant(LuaEngine &engine, const string &input, ostream &out)
{
	if (input.empty())
		return "";

	SHA1Engine sha1;
	sha1.update(input);
	const auto &digest = DigestEngine::digestToHex(sha1.digest());

	if (luaLogger().trace()) {
		luaLogger().dump(
			"chunk digest: " + digest,
			input.data(),
			input.size(),
			Message::PRIO_TRACE);
	}
	else if (luaLogger().debug()) {
		luaLogger().debug(
			"chunk digest: " + digest,
			__FILE__, __LINE__);
	}

	engine.pushLightUserData(reinterpret_cast<void *>(&out));
	engine.pushString(input);
	engine.pushCClosure(&luaCommandEchoConstant, 2);

	const auto &command = "chunk_" + digest;
	engine.setGlobal(command);

	return "\n" + command + "();";
}

/**
 * @brief Parse the given Lua code and execute it.
 */
static void luaExecute(LuaEngine &engine, const string &code)
{
	if (luaLogger().trace()) {
		luaLogger().dump(
			"loading code of size " + to_string(code.size()) + " B",
			code.data(),
			code.size(),
			Message::PRIO_TRACE);
	}
	else if (luaLogger().debug()) {
		luaLogger().debug(
			"loading code of size " + to_string(code.size()),
			__FILE__, __LINE__);
	}

	engine.loadCode(code);

	if (luaLogger().debug()) {
		luaLogger().debug(
			"executing code of size "
			+ to_string(code.size()) + " B",
			__FILE__, __LINE__);
	}

	engine.pcall(0, 0);
}

string LuaPreprocessor::process(const string &input) const
{
	LuaEngine engine;

	engine.loadMathModule();
	engine.loadStringModule();
	engine.loadTableModule();

	const auto chunks = splitChunks(input);
	ostringstream output;
	luaDefineEcho(engine, output);
	string code;

	for (const auto &chunk : chunks) {
		if (logger().trace()) {
			logger().dump(
				"inspect chunk of size "
				+ to_string(chunk.content.size()) + " B"
				+ (chunk.executable? " (executable)" : ""),
				chunk.content.data(),
				chunk.content.size(),
				Message::PRIO_TRACE);
		}
		else if (logger().debug()) {
			logger().debug(
				"inspect chunk of size "
				+ to_string(chunk.content.size()) + " B"
				+ (chunk.executable? " (executable)" : ""),
				__FILE__, __LINE__);
		}

		if (!chunk.executable)
			code += luaEchoConstant(engine, chunk.content, output);
		else
			code += chunk.content;
	}

	luaExecute(engine, code);
	return output.str();
}

vector<LuaPreprocessor::Chunk> LuaPreprocessor::splitChunks(const string &input) const
{
	vector<Chunk> chunks;
	size_t at = 0;

	if (logger().trace()) {
		logger().dump(
			"parsing input of size " + to_string(input.size()) + " B",
			input.data(),
			input.size(),
			Message::PRIO_TRACE);
	}
	else if (logger().debug()) {
		logger().debug(
			"parsing input of size " + to_string(input.size()) + " B",
			__FILE__, __LINE__);
	}

	while ((at = parseChunks(input, at, chunks)) < input.size())
		/* nothing */;

	return chunks;
}

size_t LuaPreprocessor::parseChunks(
		const string &input,
		size_t at,
		vector<Chunk> &chunks) const
{
	const auto open = input.find(MARK_OPEN, at);
	if (open == string::npos) {
		const auto &current = input.substr(at);
		chunks.emplace_back(Chunk{current, false});
		return input.size();
	}

	const auto close = input.find(MARK_CLOSE, open + MARK_OPEN.size());
	if (close == string::npos)
		throw SyntaxException("missing closing '?>' sequence");

	const auto textual = input.substr(at, open - at);
	if (!textual.empty())
		chunks.emplace_back(Chunk{textual, false});

	const auto code = input.substr(
			open + MARK_OPEN.size(),
			close - open - MARK_CLOSE.size());
	chunks.emplace_back(Chunk{code, true});

	return close + MARK_CLOSE.size();
}
