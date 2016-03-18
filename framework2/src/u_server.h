/* 
 * File:   u_server.h
 * Author: Marek Beno, xbenom01@stud.fit.vutbr.cz
 *
 * Created on 11. November 2015
 */

#ifndef U_SERVER_H
#define U_SERVER_H

#include <cstdlib>
#include <iostream>
#include <thread>

#include <boost/bind.hpp>
//#include <functional> //std::bind
#include <asio.hpp>
#include "u_session.h"



class u_server
{
public:
  u_server(asio::io_service& io_service, unsigned short port,
    std::string clientDelim, std::string serverDelim,
    int threadNum, int timePeriod);

  void run();
  void setSignals();
  void virtual startAccept() = 0;
  void handleAccept(const asio::error_code& error);
  void handleStop();
protected:
  asio::io_service& io_service_;
  asio::ip::tcp::acceptor acceptor_;
  asio::signal_set signals_;
  std::string clientDelim_;
  std::string serverDelim_;
  int threadNum_;
  int timePeriod_;
  u_session* new_session_;
};

#endif