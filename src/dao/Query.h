#pragma once

#include "dao/QueryLoader.h"
#include "dao/QueryProfiler.h"

namespace BeeeOn {

class Query {
public:
	Query(const std::string &key);
	~Query();

	void setProfiler(QueryProfiler::Ptr profiler);

	void load(QueryLoader &loader);

	std::string key() const
	{
		return m_key;
	}

	std::string get() const;

	/**
	 * Calling this method is the same as calling get()
	 * however, it adds an additional semantics. This method
	 * should be called every time when this query is used.
	 */
	std::string use();

	std::string operator ()()
	{
		return use();
	}

private:
	std::string m_key;
	std::string m_query;
	QueryProfiler::Ptr m_profiler;
};

}
