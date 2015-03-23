/* 
 * File:   IDAO.cpp
 * Author: pavel
 * 
 * Created on 16. březen 2015, 20:10
 */

#include "DAO.h"

using namespace std;
using namespace soci;

void DAO::setConnectionStringAndOpenSessions(string conString, int sessionPoolSize){

    for (size_t i = 0; i != _poolSize; ++i)
    {
        session & sql = _pool->at(i);
        sql.open(postgresql, _connectionString);
    }
}