/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ServerSession.h
 * Author: mrmaidx
 *
 * Created on 20. ledna 2016, 15:12
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

