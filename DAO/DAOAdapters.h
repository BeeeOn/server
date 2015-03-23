/* 
 * File:   DAOAdapters.h
 * Author: pavel
 *
 * Created on 17. březen 2015, 17:15
 */

#ifndef DAOADAPTERS_H
#define	DAOADAPTERS_H

#include "../ui_logger/Logger.h"
#include "Config.h"
#include "DAO.h"

class DAOAdapters : public DAO {
private:
    DAOAdapters();
    DAOAdapters(DAOAdapters const&);// Don't Implement
    void operator=(DAOAdapters const&); // Don't implement
public:
    static DAOAdapters& getInstance();
    ~DAOAdapters(void);
    
    int parAdapterWithUserIfPossible(long long int adapterId, std::string adapterName, std::string gId);
    int isAdapterInDB(long long int adapterId);
    
};

#endif	/* DAOADAPTERS_H */

