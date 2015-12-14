#ifndef SECUREDASIOSERVLET_H
#define	SECUREDASIOSERVLET_H

#define BOOST_LOG_DYN_LINK 1
#include "asio_layer/secure/server.h"

class UiServerSession : public ASIOsession
{
public:
    UiServerSession(asio::io_service& io_service, asio::ssl::context& context,
    std::string clientDelim, std::string serverDelim, int timePeriod);
    void onSession();
    void onStart();
    void onWrite();
    void onRead();
    void onExpired();
private:
};

class SecuredAsioServlet : public server
{
public:   
    SecuredAsioServlet(asio::io_service& io_service, unsigned short port,
    std::string certpath, std::string keypath,
    std::string clientDelim, std::string serverDelim,
    int threadNum, int timePeriod);
    
    void setLogging();
    void startAccept();

private:
  void handle_accept(const asio::error_code& error);

};
#endif	/* SECUREDASIOSERVLET_H */

