/* 
 * File:   UserSession.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 15. March 2016
 */

#ifndef USERSESSION_H
#define USERSESSION_H

#include "Session.h"

class UserSession: public Session {
public:
    UserSession(asio::io_service& io_service);
    
    void processMessage(std::string message) override;
};

#endif /* USERSESSION_H */