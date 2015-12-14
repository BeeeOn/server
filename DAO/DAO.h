#ifndef DAO_H
#define	DAO_H

#include <soci.h>
#include <postgresql/soci-postgresql.h>
#include "../ui_logger/Logger.h"
#include <initializer_list>
#include <cstdarg>
#include <stdint.h>
 
using gatewayId_64b = long long uint;
using deviceId_64b = long long uint;

class DAO {
public:
    virtual void setConnectionStringAndOpenSessions(std::string conString, int sessionPoolSize);
    virtual void setPool(soci::connection_pool *pool);
    
    template <typename T>
    std::string selectCols(T t) ;

    template<typename T, typename... Args>
    std::string selectCols(T t, Args... args);
    
    std::string escapeString(std::string str);
    template <typename T>
    std::string vectorToPsqlNotation(std::vector<T> vec)
    {

        std::stringstream ss;
        typename std::vector<T>::iterator it;

        for( it = vec.begin(); it < vec.end(); it++)
        {
            ss << *it;
            ss << ",";
        }
        std::string ret = ss.str();
        if(!ret.empty())
            ret.pop_back();
        
        return ret;
    }
    
private:
    
protected:
    soci::connection_pool* _pool;
};

#endif	/* DAO_H */

