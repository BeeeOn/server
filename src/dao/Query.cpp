#include <Poco/Exception.h>

#include "dao/Query.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

Query::Query(const std::string &key):
	m_key(key)
{
}

Query::~Query()
{
}

void Query::load(QueryLoader &loader)
{
	m_query = loader.find(m_key);

	if (m_query.empty())
		throw IllegalStateException("failed to load query " + m_key);
}

string Query::get() const
{
	if (m_query.empty())
		throw IllegalStateException("query " + m_key + " is not loaded");

	return m_query;
}
