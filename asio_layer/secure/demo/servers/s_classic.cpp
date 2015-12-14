#include "s_classic.h"

  s_classic_server:: s_classic_server(asio::io_service& io_service, unsigned short port,
    std::string certpath, std::string keypath,
    std::string clientDelim, std::string serverDelim,
    int threadNum, int timePeriod):
      server(io_service, port, certpath, keypath,
        clientDelim, serverDelim, threadNum, timePeriod)
      {

      }

  void s_classic_server::startAccept()
  {
    new_session_ = new s_classic_session(io_service_, context_, clientDelim_, serverDelim_, timePeriod_);
    acceptor_.async_accept(new_session_->getSocket(),
      boost::bind(&server::handle_accept, this,
      asio::placeholders::error));
  }

 s_classic_session:: s_classic_session(asio::io_service& io_service, asio::ssl::context& context,
    std::string clientDelim, std::string serverDelim, int timePeriod):
      ASIOsession(io_service, context, clientDelim, serverDelim, timePeriod)
      {

      }

  void s_classic_session::onSession()
  {
    runTimer();
    handshake();
  }
  void s_classic_session::onStart()
  {
    receive();
  }
  void s_classic_session::onRead()
  {
      renew();
      std::string msg = convertData(bytes_transferred_);
      std::cout << "Msg: " << msg << std::endl;
      std::string response = makeResponse();
      send(response); 
  }
  void s_classic_session::onWrite()
  {
    receive();
  }

  void s_classic_session::onExpired()
  {
    std::cerr << "Timer : Expired, stopping" << std::endl;
    this->isStopped = true;
    stop();
  }

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: server <port>\n";
      return 1;
    }

    std::string clientDelim("</com>");
    std::string serverDelim("</end>");
    std::string certpath("cert/ant-2.fit.vutbr.cz.pem");
    std::string keypath("cert/ant-2.fit.vutbr.cz.key"); 
    int threadCount = 10;
    int port = std::atoi(argv[1]);
    int timePeriod = 5;

    asio::io_service io_service;

    s_classic_server s(io_service, port,
      certpath, keypath,
      clientDelim, serverDelim,
      threadCount, timePeriod);
    s.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}