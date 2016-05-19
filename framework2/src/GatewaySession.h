/* 
 * File:   GatewaySession.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 29. March 2016
 */

#ifndef DATAMSGSESSION_H
#define DATAMSGSESSION_H

#include "Session.h"

class GatewaySession: public Session
{
public:
    /**
     * Constructor of class GatewaySession.
     * @param io_service Input output service.
     */
    GatewaySession(asio::io_service& io_service);
    /**
     * Processes message received from user.
     * @param message Received message.
     */
    void processMessage(std::string message) override;
};

#endif /* DATAMSGSESSION_H */
