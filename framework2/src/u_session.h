#ifndef U_SESSION_H
#define U_SESSION_H
#include <iostream>
#include <boost/bind.hpp>
#include <asio.hpp>

class u_session
{
public:
  u_session(asio::io_service& io_service, 
    std::string clientDelim, std::string serverDelim, int timePeriod);

  asio::ip::tcp::socket& getSocket();
  void runTimer();

  std::string convertData(int msg_length);
  std::string makeResponse();

  void send(std::string message);
  void receive();
  
  virtual void onStart();
  virtual void onWrite();
  virtual void onRead();
  virtual void onExpired();

  void renew();
  void expired(const asio::error_code& error);
  void stop();

protected:
  void handle_write(const asio::error_code& error);
  void handle_read(const asio::error_code& error,
      size_t bytes_transferred);

  enum { max_length = 1024 };
  asio::ip::tcp::socket socket_;
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