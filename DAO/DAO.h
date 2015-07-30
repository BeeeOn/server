/* 
 * File:   IDAO.h
 * Author: pavel
 *
 * Created on 16. březen 2015, 20:10
 */

#ifndef DAO_H
#define	DAO_H

#include <soci.h>
#include <postgresql/soci-postgresql.h>
#include "../ui_logger/Logger.h"
#include <initializer_list>
#include <cstdarg>

class DAO {
public:
    virtual void setConnectionStringAndOpenSessions(std::string conString, int sessionPoolSize);
    virtual void setPool(soci::connection_pool *pool);
    
    template <typename T>
    std::string selectCols(T t) ;

    template<typename T, typename... Args>
    std::string selectCols(T t, Args... args);
    
    std::string escapeString(std::string str);
private:

    
protected:
    //std::string _connectionString;
    //size_t _poolSize;
    soci::connection_pool* _pool;
};

#endif	/* DAO_H */

