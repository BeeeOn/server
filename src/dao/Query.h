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

	std::string operator ()() const
	{
		return get();
	}

private:
	std::string m_key;
	std::string m_query;
};

}

#endif
