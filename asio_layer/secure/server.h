#ifndef SERVER_H
#define SERVER_H

#include <cstdlib>
#include <iostream>
#include <thread>

#include <boost/bind.hpp>
#include <asio.hpp>
#include <asio/ssl.hpp>

#include "session.h"



class server
{
public:
  server(asio::io_service& io_service, unsigned short port,
    std::string certpath, std::string keypath,
    std::string clientDelim, std::string serverDelim,
    int threadNum, int timePeriod);

  void run();
  void setSignals();
  void setSSLContext(std::string certpath, std::string keypath);
  std::string get_password() const;
  void virtual startAccept() = 0;
  void handleStop();
  void handle_accept(const asio::error_code& error);
protected:


  asio::io_service& io_service_;
  asio::ip::tcp::acceptor acceptor_;
  asio::ssl::context context_;
  asio::signal_set signals_;
  std::string clientDelim_;
  std::string serverDelim_;
  int threadNum_;
  int timePeriod_;
  ASIOsession* new_session_;
};

#endif