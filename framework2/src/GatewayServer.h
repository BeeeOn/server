/* 
 * File:   GatewayServer.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 29. March 2016
 */

#ifndef DATAMSGSERVER_H
#define DATAMSGSERVER_H

#include "Server.h"

class GatewayServer: public Server
{
public:
    GatewayServer(asio::io_service& io_service, int port, int threads);   
    
    void startAccept() override;
};

#endif /* DATAMSGSERVER_H */
