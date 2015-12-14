#ifndef S_SINGLE_H
#define S_SINGLE_H

#include "../../client.h"

class s_single : public client
{
public:
  s_single(asio::io_service& io_service, asio::ssl::context& context,
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