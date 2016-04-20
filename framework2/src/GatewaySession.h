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
    GatewaySession(asio::io_service& io_service);
    
    /**
     * Process message 
     * @param message
     */
    void processMessage(std::string message) override;

};
#endif /* DATAMSGSESSION_H */
