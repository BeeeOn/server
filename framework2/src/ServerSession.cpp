/* 
 * File:   ServerSession.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
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