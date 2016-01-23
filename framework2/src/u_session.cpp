/* 
 * File:   u_session.cpp
 * Author: Marek Beno, xbenom01@stud.fit.vutbr.cz
 *
 * Created on 11. November 2015
 */

#include "u_session.h"

u_session::u_session(asio::io_service& io_service,
    std::string clientDelim, std::string serverDelim, int timePeriod)
    : socket_(io_service),
    clientDelim_(clientDelim), serverDelim_(serverDelim),
    deadline_(io_service),
    timeoutPeriod_(timePeriod)
  {
    counter_ = 0;
    isStopped = false;
  }

  asio::ip::tcp::socket& u_session::getSocket()
  {
    return socket_;
  }

  void u_session::runTimer()
  {
    this->deadline_.expires_from_now(boost::posix_time::seconds(this->timeoutPeriod_));
    this->deadline_.async_wait(boost::bind(&u_session::expired, this, _1));
  }

  std::string u_session::convertData(int msg_length)
  {
    //DBG("convertData")
    asio::streambuf::const_buffers_type bufstream = msg_buffer_.data();
    std::string response(asio::buffers_begin(bufstream), asio::buffers_begin(bufstream) + msg_length);
    // Vyčištění bufferu.
    msg_buffer_.consume(msg_length);
    return response;
  }

  std::string u_session::makeResponse()
  {
    //std::cout << "u_session::makeResponse";
    counter_++;
    std::string response = (std::to_string(counter_) + serverDelim_);
    return response;
  }

  void u_session::send(std::string message)
  {
    //std::cout << "u_session::send";
    size_t message_length = message.length();
          asio::async_write(socket_,
          asio::buffer(message, message_length),
          boost::bind(&u_session::handle_write, this,
            asio::placeholders::error));
  }

  void u_session::receive()
  {
      //std::cout << "receive";    
    asio::async_read_until(socket_, msg_buffer_, clientDelim_,
      boost::bind(&u_session::handle_read, this,
      asio::placeholders::error,
      asio::placeholders::bytes_transferred));
  }

  void u_session::onStart()
  {
    //std::cout << "u_session::onStart";
    runTimer();
    receive();
  }

  void u_session::onWrite()
  {
    //std::cout << "u_session::onWrite";
    renew();
    receive();
  }

  void u_session::onRead()
  {
      //std::cout << "u_session::onRead";
      renew();
      std::string msg = convertData(bytes_transferred_);
      std::cout << "Msg: " << msg << std::endl;
      std::string response = msg;
      send(response); 
  }

  void u_session::onExpired()
  {
    std::cerr << "Timeout timer expired" << std::endl;
    this->isStopped = true;
    stop();
  }

  void u_session::renew()
  {
    //std::cout << "u_session::renew";
    if (isStopped)
    {
      return;
    }

    int timerscancelled = this->deadline_.expires_from_now(boost::posix_time::seconds(this->timeoutPeriod_));
    if (timerscancelled > 0)
    {
      this->deadline_.async_wait(boost::bind(&u_session::expired, this, _1));
    }
    else
    {
      stop();
    }
  }  

  void u_session::expired(const asio::error_code& error)
  {
 
    if (isStopped)
    {
      //std::cerr << "Timer : Stopped before expiring" << std::endl;
      return;
    }
  
    if (!error)
    {
      onExpired();
    }
    else if (error != asio::error::operation_aborted)
    {
      std::cerr << "u_session::expired: " << error.message() << std::endl;
    }
    else
    {
      //std::cout << "u_session::expired: " << error.message();
    }

  }

  void u_session::stop()
  {
    //std::cout << "u_session::stop";
    try{
      if (socket_.is_open())
      {
        socket_.close();            
      }
  
    }
    catch (asio::system_error const& e)
    {
      std::cerr << "Session::stop exception: " << e.what() << std::endl;
    }
    std::cerr << "u_session::stop stopped, exiting" << std::endl;
  }

  void u_session::handle_write(const asio::error_code& error)
  {
    if (isStopped)
    {
      delete this;
      return;
    }

    if (!error)
    {
      onWrite();
    }
    else
    {
      std::cerr << "handle_write: " << error.message() << std::endl;
      delete this;
    }
  }

  void u_session::handle_read(const asio::error_code& error,
      size_t bytes_transferred)
  {
    if (isStopped)
    {
      delete this;
      return;
    }

    if (!error)
    {
      bytes_transferred_ = bytes_transferred;
      this->onRead();
    }
    else  //shortread
    {
      std::cerr << "handle_read: " << error.message() << std::endl;
      delete this;
    }
  }