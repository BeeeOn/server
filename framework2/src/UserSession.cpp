/* 
 * File:   UserSession.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 15. March 2016
 */

#include "UserSession.h"

 UserSession::UserSession(asio::io_service& io_service):
    Session(io_service)
{
}

void UserSession::receivedMessage(size_t bytes_transferred)
{
      //std::cout << "u_classic_session::onRead";
      //renew();
      std::string msg = convertMessage(bytes_transferred);
      std::cout << "Msg: " << msg;
      //std::string response = makeResponse();
      //send(response);
}