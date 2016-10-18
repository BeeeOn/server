#ifndef BEEEON_DEBUG_H
#define BEEEON_DEBUG_H

#if defined(__clang__) || defined(__GLIBCXX__)
#include <cxxabi.h> // abi::__cxa_demangle
#endif

#include <typeinfo>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <Poco/Exception.h>
#include <Poco/Logger.h>

namespace BeeeOn {

/**
 * Return the appropriate logger for the given class instance.
 */
#define LOGGER_CLASS(this) \
	(Poco::Logger::get(BeeeOn::classDemangle(typeid((*this)).name())))

/**
 * Return the appropriate logger for the given function name.
 */
#define LOGGER_FUNC(name)  (Poco::Logger::get((name)))

#if defined(__clang__) || defined(__GLIBCXX__)
/**
 * Use abi::__cxa_demangle to demangle the class name from typeid.
 * The abi may not be available on all compilers. The clang and
 * gcc supports this (at least in c++11).
 *
 * The classDemangle function does more then just demangling. It
 * also drops the template parameters. This leads to clear class
 * short names useful e.g. for logging configuration. Also occasional
 * pointer signs (*) are removed. Namespace prefixes are preserved.
 *
 * Example:
 * Test<Other> -> 4TestI5OtherE -> Test
 * Other -> 5Other -> Other
 * Other * -> P5Other -> Other
 * ?? -> PN6BeeeOn18DependencyInjectorE
 *    -> BeeeOn::DependencyInjector* -> BeeeOn::DependencyInjector
 *
 * The ?? comes probably from this pointer.
 */
static inline const std::string classDemangle(const char *name)
{
	char *demangled = abi::__cxa_demangle(name, 0, 0, 0);
	std::string s(demangled);
	std::free(demangled);

	std::size_t t;

	t = s.find("*<");
	if (t != std::string::npos)
		return s.substr(0, t);

	t = s.find("<");
	if (t != std::string::npos) {
		std::size_t star = s.find("*");
		if (star == std::string::npos)
			return s.substr(0, t);

		return s.substr(0, star < t? star : t);
	}

	t = s.find("*");
	return t == std::string::npos? s : s.substr(0, t);
}
#else

/**
 * For unknown compilers, return the unmangled name because we
 * don't know what can we do.
 */
static inline const std::string classDemangle(const std::string &name)
{
	return name;
}
#endif

/**
 * Tracing can be completele removed by specifying BEEEON_NO_TRACE
 * during compilation.
 */
#ifndef BEEEON_NO_TRACE

/**
 * Tracer for automatic tracing of function calls. If instantiated
 * at the beginning of a function, it logs the enter and leave
 * events.
 */
class Trace {
public:
	Trace(Poco::Logger &logger, const char *func,
			const char *file, int line):
		m_logger(logger),
		m_func(func),
		m_file(file),
		m_line(line)
	{
		if (m_logger.trace()) {
			m_logger.trace(
				Poco::Logger::format("enter $0", std::string(m_func)),
				m_file, m_line);
		}
	}

	~Trace()
	{
		if (m_logger.trace()) {
			m_logger.trace(
				Poco::Logger::format("leave $0", std::string(m_func)),
				m_file, 0);
		}
	}

private:
	Poco::Logger &m_logger;
	std::string m_func;
	const char *m_file;
	int m_line;
};

#define _INTERNAL_TRACE_METHOD(logger) BeeeOn::Trace \
	_trace((logger), __func__, __FILE__, __LINE__)

#define _INTERNAL_TRACE_FUNC(logger) BeeeOn::Trace \
	__trace((logger), __func__, __FILE__, __LINE__)

#else // BEEEON_NO_TRACE

#define _INTERNAL_TRACE_METHOD(logger)
#define _INTERNAL_TRACE_FUNC(logger)

#endif // BEEEON_NO_TRACE

/**
 * Trace a method of the class instance "this".
 * To reuse an existing logger, call _TRACE_METHOD(logger).
 */
#define TRACE_METHOD() \
	_INTERNAL_TRACE_METHOD(LOGGER_CLASS(this))

/**
 * Trace with a custom logger given by caller.
 */
#define _TRACE_METHOD(logger) _INTERNAL_TRACE_METHOD(logger)

/**
 * Trace the current function.
 * To reuse an existing logger, call _TRACE_FUNC(logger).
 */
#define TRACE_FUNC() \
	_INTERNAL_TRACE_FUNC(LOGGER_FUNC(__func__))

/**
 * Trace with a custom logger given by caller.
 */
#define _TRACE_FUNC(logger) _INTERNAL_TRACE_FUNC(logger)

}

#ifdef __GLIBC__
#include <execinfo.h>
#include <cstdio>

namespace BeeeOn {

/**
 * Print backtrace of the current function call hierarchy
 * to stdout. Useful for very unexpected states and exceptions
 * and when even the logging facilities fail.
 */
inline void stdout_backtrace()
{
	void *addr[128];
	size_t count = backtrace(addr, 128);

	backtrace_symbols_fd(addr, count, STDOUT_FILENO);
}

/**
 * Log backtrace of the current function call hierarchy
 * to stdout. Useful for very unexpected states and exceptions.
 * All lines are logged as critical and with prefix "Backtrace: ".
 */
inline void __log_backtrace(Poco::Logger &l, const char *file, size_t line)
{
	void *addr[128];
	size_t count = backtrace(addr, 128);
	char **strings = backtrace_symbols(addr, count);
	size_t i;

	l.critical(
		Poco::Logger::format("Backtrace: size $0", std::to_string(count)),
		file, line);

	for (i = 0; i < count; ++i) {
		l.critical(Poco::Logger::format(
				"Backtrace: > $0", std::string(strings[i])),
			file, line);
	}
}

}

#else // unknown libc/c++ library

namespace BeeeOn {

/**
 * No idea how to support backtracing for an unknown libc/c++ library.
 */
inline void stdout_backtrace()
{
	const char *msg = "no backtrace available\n";
	write(STDOUT_FILENO, msg, sizeof(msg));
}

/**
 * No idea how to support backtracing for an unknown libc/c++ library.
 */
inline void __log_backtrace(Poco::Logger &l, const char *file, size_t line)
{
	l.critical("Backtrace: not available", file, line);
}

}

#endif

/**
 * Log backtrace to the given logger with level critical.
 */
#define log_backtrace(l) BeeeOn::__log_backtrace(l, __FILE__, __LINE__)

#endif
