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
    /**
     * Constructor of class UserSession.
     * @param io_service Input output service.
     */
    UserSession(asio::io_service& io_service);
    /**
     * Processes message received from user.
     * @param message Received message.
     */
    void processMessage(std::string message) override;
    /**
     * Check if user owns instance in database.
     * @param user_id ID of checked user.
     * @param instance_id ID of checked instance.
     */    
    void checkIfUserOwnsInstance(long user_id, long instance_id);
};

#endif /* USERSESSION_H */
