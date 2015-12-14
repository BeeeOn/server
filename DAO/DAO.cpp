#include "DAO.h"

using namespace std;
using namespace soci;

void DAO::setConnectionStringAndOpenSessions(string conString, int sessionPoolSize)
{
    _pool = new connection_pool(sessionPoolSize);
    for (int i = 0; i != sessionPoolSize; ++i)
    {
        session & sql = _pool->at(i);
        sql.open(postgresql, conString);
    }
}

void DAO::setPool(soci::connection_pool* pool) 
{
    _pool = pool;
}

string DAO::escapeString(string str) 
{
    char escaped[300];
    PQescapeString(escaped, str.c_str(), 300);
    return escaped;
}

template <typename T>
string DAO::selectCols(T t) 
{
    return t;
}

template<typename T, typename... Args>
string DAO::selectCols(T t, Args... args)
{
    return  t + "," + selectCols(args...) ;
}

