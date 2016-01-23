/* 
 * File:   ServerSession.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef SERVERSESSION_H
#define SERVERSESSION_H

#include "u_session.h"

class ServerSession : public u_session
{
public:
    ServerSession(asio::io_service& io_service,
    std::string clientDelim, std::string serverDelim, int timePeriod);
    void onStart();
    //void onWrite();
    void onRead();
    void onExpired();
};

#endif /* SERVERSESSION_H */

