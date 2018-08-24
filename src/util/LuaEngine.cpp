#include <Poco/Exception.h>

#include <lua.hpp>

#include "util/LuaEngine.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

#ifndef LUA_OK
#define LUA_OK 0
#endif

LuaEngine::LuaEngine():
	m_own(true)
{
	m_state = luaL_newstate();
	if (m_state == nullptr)
		throw IllegalStateException("failed to create Lua state");
}

LuaEngine::LuaEngine(struct lua_State *state, bool own):
	m_state(state),
	m_own(own)
{
}

LuaEngine::~LuaEngine()
{
	if (m_own)
		lua_close(m_state);
}

uint32_t LuaEngine::version()
{
#if LUA_VERSION_NUM > 501
	const auto *ver = lua_version(m_state);
	return static_cast<uint32_t>(*ver);
#else
	return LUA_VERSION_NUM;
#endif
}

void LuaEngine::loadPackageModule()
{
	loadModule("package", &luaopen_package);
}

void LuaEngine::loadMathModule()
{
	loadModule("math", &luaopen_math);
}

void LuaEngine::loadStringModule()
{
	loadModule("string", &luaopen_string);
}

void LuaEngine::loadTableModule()
{
	loadModule("table", &luaopen_table);
}

void LuaEngine::loadCoroutineModule()
{
#if LUA_VERSION_NUM > 501
	loadModule("coroutine", &luaopen_coroutine);
#else
	throw NotImplementedException(__func__);
#endif
}

void LuaEngine::loadDebugModule()
{
	loadModule("debug", &luaopen_debug);
}

void LuaEngine::loadIOModule()
{
	loadModule("io", &luaopen_io);
}

void LuaEngine::loadOSModule()
{
	loadModule("os", &luaopen_os);
}

void LuaEngine::loadModule(
		const string &name,
		CFunction loader)
{
	lua_pushcfunction(m_state, loader);
	lua_pushstring(m_state, name.c_str());
	lua_call(m_state, 1, 0);
}

void LuaEngine::pushCClosure(CFunction f, int size)
{
	lua_pushcclosure(m_state, f, size);
}

void LuaEngine::pushCFunction(CFunction f)
{
	lua_pushcfunction(m_state, f);
}

void LuaEngine::pushBool(bool b)
{
	lua_pushboolean(m_state, b);
}

void LuaEngine::pushInt(int i)
{
	lua_pushinteger(m_state, i);
}

void LuaEngine::pushLightUserData(void *d)
{
	lua_pushlightuserdata(m_state, d);
}

void LuaEngine::pushString(const string &s)
{
	lua_pushlstring(m_state, s.c_str(), s.size());
}

void LuaEngine::pushNil()
{
	lua_pushnil(m_state);
}

bool LuaEngine::toBool(int stack)
{
	return lua_toboolean(m_state, stack);
}

int LuaEngine::toInt(int stack)
{
	return lua_tointeger(m_state, stack);
}

LuaEngine::CFunction LuaEngine::toCFunction(int stack)
{
	return lua_tocfunction(m_state, stack);
}

std::string LuaEngine::toString(int stack)
{
	size_t len = 0;
	const char *s = lua_tolstring(m_state, stack, &len);

	if (s != nullptr)
		return string(s, len);

	const auto type = lua_type(m_state, stack);
	const string &name = lua_typename(m_state, type);

	throw IllegalStateException(
		"cannot convert entry " + to_string(stack)
		+ " of type " + name + " to string");
}

void *LuaEngine::toUserData(int stack)
{
	void *d = lua_touserdata(m_state, stack);
	if (d != nullptr)
		return d;

	const auto type = lua_type(m_state, stack);
	const string &name = lua_typename(m_state, type);

	throw IllegalStateException(
		"cannot convert entry " + to_string(stack)
		+ " of type " + name + " to string");
}

void LuaEngine::setGlobal(const string &name)
{
	lua_setglobal(m_state, name.c_str());
}

int LuaEngine::type(int stack)
{
	return lua_type(m_state, stack);
}

string LuaEngine::typeName(int stack)
{
	return lua_typename(m_state, lua_type(m_state, stack));
}

void LuaEngine::pop(int count)
{
	lua_pop(m_state, count);
}

void LuaEngine::loadCode(const string &code)
{
	const auto err = luaL_loadstring(m_state, code.c_str());
	throwErrorIfAny(err);
}

void LuaEngine::pcall(int args, int results)
{
	const auto err = lua_pcall(m_state, args, results, 0);
	throwErrorIfAny(err);
}

string LuaEngine::errorMessage(const int err)
{
	if (err == LUA_OK)
		return "ok";

	if (lua_gettop(m_state) > 0)
		return lua_tostring(m_state, -1);

	return "Lua failed with error " + to_string(err);
}

void LuaEngine::throwErrorIfAny(const int err)
{
	switch (err) {
	case LUA_OK:
		return;

	case LUA_ERRSYNTAX:
		throw SyntaxException(errorMessage(err));

	case LUA_ERRMEM:
		throw OutOfMemoryException(errorMessage(err));

	default:
		throw RuntimeException(errorMessage(err));
	}
}

struct lua_State *LuaEngine::state()
{
	return m_state;
}

int LuaEngine::upvalueIndex(int index)
{
	return lua_upvalueindex(index);
}
