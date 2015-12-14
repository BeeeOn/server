#include "SecuredAsioServlet.h"

SecuredAsioServlet:: SecuredAsioServlet(asio::io_service& io_service, unsigned short port,
    std::string certpath, std::string keypath,
    std::string clientDelim, std::string serverDelim,
    int threadNum, int timePeriod):
      server(io_service, port, certpath, keypath,
        clientDelim, serverDelim, threadNum, timePeriod)
      {

      }

  void SecuredAsioServlet::startAccept()
  {
    new_session_ = new UiServerSession(io_service_, context_, clientDelim_, serverDelim_, timePeriod_);
    
    std::cout << "start Accept" << std::endl;
    
    std::cout << new_session_ << std::endl;
    acceptor_.async_accept(new_session_->getSocket(),
      boost::bind(&server::handle_accept, this,
      asio::placeholders::error));
  }

 UiServerSession:: UiServerSession(asio::io_service& io_service, asio::ssl::context& context,
    std::string clientDelim, std::string serverDelim, int timePeriod):
      ASIOsession(io_service, context, clientDelim, serverDelim, timePeriod)
      {

      }

  void UiServerSession::onSession()
  {
    std::cout << "on session " << std::endl;
    runTimer();
    handshake();
  }
  void UiServerSession::onStart()
  {
    receive();
  }
  void UiServerSession::onRead()
  {
      renew();
      std::string msg = convertData(bytes_transferred_);
      std::cout << "ui Msg: " << msg << std::endl;
//      std::string response = makeResponse();
      std::string response = "ASIO: hello";
      send(response); 
  }
  void UiServerSession::onWrite()
  {
    receive();
  }

  void UiServerSession::onExpired()
  {
    std::cerr << "Timer : Expired, stopping" << std::endl;
    this->isStopped = true;
    stop();
  }