/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ServerSession.cpp
 * Author: mrmaidx
 * 
 * Created on 20. ledna 2016, 15:12
 */

#include "ServerSession.h"

 ServerSession::ServerSession(asio::io_service& io_service,
    std::string clientDelim, std::string serverDelim, int timePeriod):
      u_session(io_service, clientDelim, serverDelim, timePeriod)
      {

      }

  void ServerSession::onStart()
  {
    //std::cout << "u_classic_session::onStart";
    runTimer();
    receive();
  }
/*
  void ServerSession::onWrite()
  {
    receive();
  }
*/
  void ServerSession::onRead()
  {
      //std::cout << "u_classic_session::onRead";
      renew();
      std::string msg = convertData(bytes_transferred_);
      std::cout << "Msg: " << msg << std::endl;
      //std::string response = makeResponse();
      //send(response);
  }

  void ServerSession::onExpired()
  {
    std::cerr << "Timer : Expired, stopping" << std::endl;
    this->isStopped = true;
    stop();
  }