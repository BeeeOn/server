#ifndef BEEEON_SQL_QUERY_H
#define BEEEON_SQL_QUERY_H

#include "dao/SQLLoader.h"

namespace BeeeOn {

class SQLQuery {
public:
	SQLQuery(const std::string &key);
	~SQLQuery();

	void load(SQLLoader &loader);

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
