#ifndef BEEEON_QUERY_H
#define BEEEON_QUERY_H

#include "dao/QueryLoader.h"

namespace BeeeOn {

class Query {
public:
	Query(const std::string &key);
	~Query();

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
};

}

#endif
