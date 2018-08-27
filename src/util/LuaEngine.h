#pragma once

#include <string>

struct lua_State;

namespace BeeeOn {

/**
 * @brief Wrapper around the Lua state. It allows to execute Lua
 * code and work with the Lua stack.
 */
class LuaEngine {
public:
	typedef int (*CFunction)(struct lua_State *);

	LuaEngine();
	LuaEngine(struct lua_State *state, bool own = true);
	~LuaEngine();

	uint32_t version();

	void loadPackageModule();
	void loadMathModule();
	void loadStringModule();
	void loadTableModule();
	void loadCoroutineModule();
	void loadDebugModule();
	void loadIOModule();
	void loadOSModule();
	void loadModule(const std::string &name, CFunction loader);

	void pushCClosure(CFunction f, int size);
	void pushCFunction(CFunction f);
	void pushBool(bool b);
	void pushInt(int i);
	void pushLightUserData(void *d);
	void pushString(const std::string &s);
	void pushNil();

	bool toBool(int stack = -1);
	int toInt(int stack = -1);
	CFunction toCFunction(int stack = -1);
	std::string toString(int stack = -1);
	void *toUserData(int stack = -1);

	void setGlobal(const std::string &name);

	int type(int stack = -1);
	std::string typeName(const int type);

	void pop(int count);

	void loadCode(const std::string &code);
	void pcall(int args, int results);

	std::string errorMessage(const int err);
	void throwErrorIfAny(const int err);

	struct lua_State *state();

	static int upvalueIndex(int index);

private:
	struct lua_State *m_state;
	bool m_own;
};

}
