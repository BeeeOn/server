#include "DBConnector.h"

using namespace soci;
using namespace std;
#define COM_VERSION "1.0.2"
#define DBTIMEFORMAT "YYYY-MM-DD HH24:MI:SS"
DBConnector::DBConnector()
{
}


DBConnector::~DBConnector(void)
{
    this->clean();
}

DBConnector& DBConnector::getInstance(){
        static DBConnector instance;
        return instance;
}

soci::session* DBConnector::getSession() {
    session* s = new session(*_pool);
    return s;    
}

void DBConnector::releaseSession(soci::session* session) {
    delete session;
    return;
}


void DBConnector::setConnectionStringAndOpenSessions(string conString, int sessionPoolSize){
    _connectionString = conString;
    _poolSize = sessionPoolSize;
    _pool = new connection_pool(_poolSize);

    for (size_t i = 0; i != _poolSize; ++i)
    {
        session & sql = _pool->at(i);
        sql.open(postgresql, _connectionString);
    }
}

void DBConnector::DEBUGsetConnectionStringAndOpenSessions(string conString, int sessionPoolSize){
    _connectionString = conString;
       //TODO pozor na deadlock, pokud nekde jsou 2 sql sessions pouzite za sebou
    _poolSize = sessionPoolSize;
    _pool = new connection_pool(_poolSize);

    for (size_t i = 0; i != _poolSize; ++i)
    {
        session & sql = _pool->at(i);
        sql.open(postgresql, _connectionString);
        sql.begin();
    }
}
void DBConnector::DEBUGrollbackEverything(){
    for (size_t i = 0; i != _poolSize; ++i)
    {
        session & sql = _pool->at(i);
        sql.rollback();
    }
}

string DBConnector::DEBUGexec(string sqlQuery){
    session sql(*_pool);
    string result;
    cout<<"exec: "<<sqlQuery<<endl;;
    try{
        sql << sqlQuery,into(result);
    }
    catch (soci::postgresql_soci_error& e)
    {
        cerr<<"ERROR in DEBUGexec "<<sqlQuery<<endl;
        cerr<<"cause "<< e.what()<<endl;
    }
    cout<<"exec result:"<<result<<endl;
    return result;
}

string DBConnector::escapeString(string str) {
    char escaped[300];
    PQescapeString(escaped, str.c_str(), 300);
    return escaped;
}


void DBConnector::clean(){
     for (size_t i = 0; i != _poolSize; ++i)
    {
        session & sql = _pool->at(i);
        sql.close();
    }
     delete _pool;
}