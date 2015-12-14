#ifndef S_TIMEOUT_H
#define S_TIMEOUT_H
#include "../../client.h"

class s_timeout : public client
{
public:
  s_timeout(asio::io_service& io_service, asio::ssl::context& context,
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