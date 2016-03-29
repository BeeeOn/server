/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DataMsgSession.h
 * Author: mrmaidx
 *
 * Created on 29. března 2016, 10:47
 */

#ifndef DATAMSGSESSION_H
#define DATAMSGSESSION_H

#include "Session.h"

class GatewaySession: public Session
{
public:
    GatewaySession(asio::io_service& io_service);
    
    void receivedMessage(size_t bytes_transferred) override;

};

#endif /* DATAMSGSESSION_H */