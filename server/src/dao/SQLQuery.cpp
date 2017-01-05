#include <Poco/Exception.h>

#include "dao/SQLQuery.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

SQLQuery::SQLQuery(const std::string &key):
	m_key(key)
{
}

SQLQuery::~SQLQuery()
{
}

void SQLQuery::load(SQLLoader &loader)
{
	m_query = loader.find(m_key);

	if (m_query.empty())
		throw IllegalStateException("failed to load query " + m_key);
}

string SQLQuery::get() const
{
	if (m_query.empty())
		throw IllegalStateException("query " + m_key + " is not loaded");

	return m_query;
}
