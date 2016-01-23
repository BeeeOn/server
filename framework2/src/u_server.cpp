/* 
 * File:   u_server.cpp
 * Author: Marek Beno, xbenom01@stud.fit.vutbr.cz
 *
 * Created on 11. November 2015
 */

#include "u_server.h"

  u_server::u_server(asio::io_service& io_service, unsigned short port,
    std::string clientDelim, std::string serverDelim,
    int threadNum, int timePeriod)
    : io_service_(io_service),
      acceptor_(io_service,
        asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
        signals_(io_service),
        clientDelim_(clientDelim),
        serverDelim_(serverDelim),
        threadNum_(threadNum),
        timePeriod_(timePeriod)
  {
    //std::cout << "Server started on port: " << port << std::endl;
    setSignals();
  }

  void u_server::run()
  {
    std::vector<std::shared_ptr<std::thread> > threads;

    for (std::size_t i = 0; i < threadNum_; ++i) 
    {
      std::shared_ptr<std::thread> thread(new std::thread(
        boost::bind(&asio::io_service::run, &io_service_)));
      threads.push_back(thread);
    }

    for (std::size_t i = 0; i < threads.size(); ++i)
    {
      threads[i]->join();
    }
  }

  void u_server::setSignals()
  {
    signals_.add(SIGINT);
    signals_.add(SIGTERM);
      #if defined(SIGQUIT)
    signals_.add(SIGQUIT);
      #endif // defined(SIGQUIT)
    signals_.async_wait(boost::bind(&u_server::handleStop, this));
  }

  void u_server::handleAccept(const asio::error_code& error)
  {
    //std::cout << "u_server::handleAccept";
    if (!error)
    {
      new_session_->onStart();
      startAccept();
    }
    else
    {
    std::cerr << "handleAccept error" << error.message() << std::endl;
      delete new_session_;
    }
  }

  void u_server::handleStop() 
  {
    std::cerr << "u_server::handleStop: Server stopped" << std::endl;
    io_service_.stop();
    delete new_session_;
    //delete this;
  }