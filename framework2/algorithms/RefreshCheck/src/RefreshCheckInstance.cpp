/* 
 * File:   RefreshInstance.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 21. January 2016
 */

#include "RefreshCheckInstance.h"

#include <iostream>

//#include <soci.h>
//#include <postgresql/soci-postgresql.h>

//using namespace soci;

RefreshCheckInstance::RefreshCheckInstance(unsigned long user_id, unsigned int users_instance_personal_id):
    TimedTaskInstance(user_id, users_instance_personal_id)
{
}

RefreshCheckInstance::~RefreshCheckInstance() {
    std::cout << "RefreshCheckInstance::~RefreshCheckInstance - entered." << std::endl;
    std::cout << "RefreshCheckInstance::~RefreshCheckInstance - finished." << std::endl;
}

void RefreshCheckInstance::run() {
    planActivationAfterSeconds(5);
    
    executeRefreshQuery();
}

void RefreshCheckInstance::executeRefreshQuery() {
    
    std::cout << "PLAY REFRESH CHECK" << std::endl;
    /*
    session sql(postgresql, "port = '5432' dbname = 'home7' user = 'uiserver7' password = '1234' connect_timeout = '3'");
  
    std::cout << "Update devices ... ";
    
    sql << "UPDATE device SET status = CASE measured_at + 3 * get_refresh(gateway_id, device_euid) > extract(epoch from now()) "
           "WHEN true THEN 'available'::device_status "                                                                                                                                                  
           "WHEN false THEN 'unavailable'::device_status " 
           "ELSE 'available'::device_status "
           "END;";
    
    std::cout << "done" << std::endl;
    */
}
     