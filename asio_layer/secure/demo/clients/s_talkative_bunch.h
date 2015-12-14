#ifndef S_TALKATIVE_BUNCH_H
#define S_TALKATIVE_BUNCH_H

#include "../../client.h"

class talkative_bunch : public client
{
public:
  talkative_bunch(asio::io_service& io_service, asio::ssl::context& context,
    std::string host, std::string port,
    std::string clientDelim, std::string serverDelim);

    std::string convertData(int reply_length);
    std::string getMessage();

    void onStart();
    void onWrite();
    void onRead(); 
    int local_counter;
};

#endif