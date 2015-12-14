#ifndef S_CLASSIC_H
#define S_CLASSIC_H

#define BOOST_LOG_DYN_LINK 1
#include "../../server.h"

class s_classic_session : public ASIOsession
{
public:
    s_classic_session(asio::io_service& io_service, asio::ssl::context& context,
    std::string clientDelim, std::string serverDelim, int timePeriod);
    void onSession();
    void onStart();
    void onWrite();
    void onRead();
    void onExpired();
private:
};

class s_classic_server : public server
{
public:   
    s_classic_server(asio::io_service& io_service, unsigned short port,
    std::string certpath, std::string keypath,
    std::string clientDelim, std::string serverDelim,
    int threadNum, int timePeriod);
    
    void setLogging();
    void startAccept();

private:
  void handle_accept(const asio::error_code& error);

    s_classic_session* new_session_;
};

#endif