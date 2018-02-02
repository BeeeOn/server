#pragma once

#include <string>

#include <Poco/SharedPtr.h>

namespace BeeeOn {

/**
 * @brief QueryProfiler allows to watch performing of queries.
 */
class QueryProfiler {
public:
	typedef Poco::SharedPtr<QueryProfiler> Ptr;

	virtual ~QueryProfiler();

	/**
	 * Notify profiler that a query is being used.
	 */
	virtual void use(const std::string &key) = 0;
};

}
