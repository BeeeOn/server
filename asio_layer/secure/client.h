#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <asio.hpp>
#include <asio/ssl.hpp>

//#include "../debug.h"


enum { max_length = 1024 };


class client
{
public:
    client(asio::io_service& io_service, asio::ssl::context& context,
        std::string host, std::string port,
        std::string clientDelim, std::string serverDelim);

    virtual void onStart() = 0;

    virtual void onWrite() = 0;

    virtual void onRead() = 0;

    void connect();
    void handshake();
    void send(std::string request);
    void receive();



protected:
    std::string clientDelim_;
    std::string serverDelim_;
    std::string request_;
    std::string reply_;
    size_t bytes_transferred_;
    asio::streambuf reply_buffer_;
private:
    asio::ssl::stream<asio::ip::tcp::socket> socket_;
    asio::ip::tcp::resolver resolver_;
    std::string host_;
    std::string port_;

    void handle_connect(const asio::error_code& error);
    void handle_handshake(const asio::error_code& error);
    void handle_write(const asio::error_code& error,
        size_t bytes_transferred);
    void handle_read(const asio::error_code& error,
        size_t bytes_transferred);

};