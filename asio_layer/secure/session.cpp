#include "session.h"

ASIOsession::ASIOsession(asio::io_service& io_service, asio::ssl::context& context,
    std::string clientDelim, std::string serverDelim, int timePeriod)
    : socket_(io_service, context),
    clientDelim_(clientDelim), serverDelim_(serverDelim),
    deadline_(io_service),
    timeoutPeriod_(timePeriod)
  {
    counter_ = 0;
    isStopped = false;
  }

  ssl_socket::lowest_layer_type& ASIOsession::getSocket()
  {
    return socket_.lowest_layer();
  }



  void ASIOsession::runTimer()
  {
    //DBG("runTimer")
    this->deadline_.expires_from_now(boost::posix_time::seconds(this->timeoutPeriod_));
    this->deadline_.async_wait(boost::bind(&ASIOsession::expired, this, _1));
  }

  void ASIOsession::handshake()
  {
    socket_.async_handshake(asio::ssl::stream_base::server,
      boost::bind(&ASIOsession::handle_handshake, this,
      asio::placeholders::error));
  }



  void ASIOsession::handle_handshake(const asio::error_code& error)
  {
    if (isStopped)
    {
      //std::cerr << "Stopped before handle_handshake" << std::endl;
      delete this;
      return;
    }

    if (!error)
    {
      onStart();
    }
    else
    {
      std::cerr << "handle_handshake: " << error.message() << std::endl;
      delete this;
      return;
    }
  }

  void ASIOsession::receive()
  {
    asio::async_read_until(socket_, msg_buffer_, clientDelim_,
      boost::bind(&ASIOsession::handle_read, this,
      asio::placeholders::error,
      asio::placeholders::bytes_transferred));
  }

  std::string ASIOsession::convertData(int msg_length)
  {
    //DBG("convertData")
    asio::streambuf::const_buffers_type bufstream = msg_buffer_.data();
    std::string response(asio::buffers_begin(bufstream), asio::buffers_begin(bufstream) + msg_length);
    // Vyčištění bufferu.
    msg_buffer_.consume(msg_length);
    return response;
  }



  void ASIOsession::handle_read(const asio::error_code& error,
      size_t bytes_transferred)
  {
    if (isStopped)
    {
      //std::cerr << "Stopped before handle_read" << std::endl;
      delete this;
      return;
    }

    if (!error)
    {
      bytes_transferred_ = bytes_transferred;
      this->onRead();
    }
    else
    {
      std::cerr << "handle_read: " << error.message() << std::endl;
      delete this;
    }
  }

  void ASIOsession::send(std::string message)
  {
    size_t message_length = message.length();
          asio::async_write(socket_,
          asio::buffer(message, message_length),
          boost::bind(&ASIOsession::handle_write, this,
            asio::placeholders::error));
  }

  std::string ASIOsession::makeResponse()
  {
    counter_++;
    std::string response = (std::to_string(counter_) + serverDelim_);
    return response;
  }



  void ASIOsession::handle_write(const asio::error_code& error)
  {
    //DBG("handle_write")
      if (isStopped)
    {
      //std::cerr << "Stopped before handle_write" << std::endl;
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

  void ASIOsession::renew()
  {
    
    if (isStopped)
    {
      //std::cerr << "Timer : Stopped before renewing" << std::endl;
      return;
    }
    
    //DBG("renew")
    int timerscancelled = this->deadline_.expires_from_now(boost::posix_time::seconds(this->timeoutPeriod_));
    if (timerscancelled > 0)
    {
      //timer cancelled  
      this->deadline_.async_wait(boost::bind(&ASIOsession::expired, this, _1));
    }
    else
    {
      std::cerr << "Timer : Error: Can't renew, timer expired" << std::endl;
      stop();
    }
  }

  void ASIOsession::onSession()
  {
    runTimer();
    handshake();
  }
  void ASIOsession::onStart()
  {
    receive();
  }
  void ASIOsession::onRead()
  {
      renew();
      std::string msg = convertData(bytes_transferred_);
      std::cout << "Msg: " << msg << std::endl;
      std::string response = msg;
      send(response); 
  }
  void ASIOsession::onWrite()
  {
    receive();
  }

  void ASIOsession::onExpired()
  {
    //std::cerr << "Timer : Expired, stopping" << std::endl;
    this->isStopped = true;
    stop();
  }

  void ASIOsession::expired(const asio::error_code& error)
  {
    //DBG("expired")
  
  if (isStopped)
  {
    //std::cerr << "Timer : Stopped before expiring" << std::endl;
    return;
  }
  
    if (error != asio::error::operation_aborted)
    {
      onExpired();
    }
  }

  void ASIOsession::stop()
  {
    std::cerr << "Session stopped" << std::endl;
    try{
      socket_.shutdown();      
    }
    catch (asio::system_error const& e)
    {
      std::cerr << "Session::stop exception: " << e.what() << std::endl;
    }

    //delete this;
  }