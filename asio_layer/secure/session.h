#ifndef SESSION_H
#define SESSION_H

#include <iostream>
#include <boost/bind.hpp>
#include <asio.hpp>
#include <asio/ssl.hpp>



typedef asio::ssl::stream<asio::ip::tcp::socket> ssl_socket;


class ASIOsession
{
public:
  ASIOsession(asio::io_service& io_service, asio::ssl::context& context,
    std::string clientDelim, std::string serverDelim, int timePeriod);

  ssl_socket::lowest_layer_type& getSocket();

  std::string convertData(int msg_length);
  std::string makeResponse();

  virtual void onSession();
  virtual void onStart();
  virtual void onWrite();
  virtual void onRead();
  virtual void onExpired();

  void runTimer();
  void handshake();

  void send(std::string message);
  void receive();
  
  void renew();
  void expired(const asio::error_code& error);
  void stop();

protected:
  void handle_handshake(const asio::error_code& error);
  void handle_write(const asio::error_code& error);
  void handle_read(const asio::error_code& error,
      size_t bytes_transferred);

  ssl_socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];
  std::string response_;
  int counter_;
  std::string clientDelim_;
  std::string serverDelim_;
  asio::streambuf msg_buffer_;
  asio::deadline_timer deadline_;
  int timeoutPeriod_;
  bool isStopped;
  size_t bytes_transferred_;
};

#endif 