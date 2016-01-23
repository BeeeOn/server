/* 
 * File:   Server.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef SERVER_H
#define SERVER_H

#include "u_server.h"

class Server : public u_server
{
public:   
    Server(asio::io_service& io_service, unsigned short port,
    std::string clientDelim, std::string serverDelim,
    int threadNum, int timePeriod);   
    void startAccept();
};

#endif /* SERVER_H */
