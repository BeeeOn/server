#include "server.h"

  server::server(asio::io_service& io_service, unsigned short port,
    std::string certpath, std::string keypath,
    std::string clientDelim, std::string serverDelim,
    int threadNum, int timePeriod)
    : io_service_(io_service),
      acceptor_(io_service,
        asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
        context_(io_service, asio::ssl::context::sslv23),
        signals_(io_service),
        clientDelim_(clientDelim),
        serverDelim_(serverDelim),
        threadNum_(threadNum),
        timePeriod_(timePeriod)
  {
    std::cout << "Server started on port: " << port << std::endl;
    setSignals();
    try
    {
      setSSLContext(certpath, keypath);
    }
    catch (asio::system_error const& e)
    {
      std::cerr << "setSSLContext exception: " << e.what() << std::endl;
      handleStop();
    }
  }

  void server::run()
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

  void server::setSignals()
  {
    signals_.add(SIGINT);
    signals_.add(SIGTERM);
      #if defined(SIGQUIT)
    signals_.add(SIGQUIT);
      #endif // defined(SIGQUIT)
    signals_.async_wait(boost::bind(&server::handleStop, this));
  }

  void server::setSSLContext(std::string certpath, std::string keypath)
  {
    context_.set_options(
        asio::ssl::context::default_workarounds
        | asio::ssl::context::no_sslv2
        | asio::ssl::context::single_dh_use);
    context_.set_password_callback(boost::bind(&server::get_password, this));
    context_.use_certificate_chain_file(certpath);
    context_.use_private_key_file(keypath, asio::ssl::context::pem);
    //context_.use_tmp_dh_file("dh512.pem");
  }

  std::string server::get_password() const
  {
    return "test";
  }

//  void server::startAccept()
//  {
//
//  }

  void server::handle_accept(const asio::error_code& error)
  {
    std::cout << "handle accept" << std::endl;
    if (!error)
    {
        std::cout << new_session_ << std::endl;
      new_session_->onSession();
      startAccept();
    }
    else
    {
      std::cerr << "handle_accept error" << error.message() << std::endl;
      delete new_session_;
    }
  }

  void server::handleStop() 
  {
    std::cout << "Server stopped." << std::endl;
    io_service_.stop();
    delete new_session_;
    //delete this;
  }