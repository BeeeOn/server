#include "client.h"

  client::client(asio::io_service& io_service, asio::ssl::context& context,
    std::string host, std::string port,
    std::string clientDelim, std::string serverDelim)
    : socket_(io_service, context),
    resolver_(io_service),
    host_(host), port_(port),
    clientDelim_(clientDelim), serverDelim_(serverDelim)
  {
  }

  void client::connect()
  {
    asio::ip::tcp::endpoint endpoint = *(resolver_.resolve({host_, port_}));
    socket_.lowest_layer().async_connect(endpoint,
      boost::bind(&client::handle_connect, this,
      asio::placeholders::error));
  }

  void client::handshake()
  {
    socket_.async_handshake(asio::ssl::stream_base::client,
    boost::bind(&client::handle_handshake, this,
    asio::placeholders::error));
  }

  void client::send(std::string request)
  {
    size_t request_length = request.length();
    asio::async_write(socket_,
      asio::buffer(request, request_length),
      boost::bind(&client::handle_write, this,
      asio::placeholders::error,
      asio::placeholders::bytes_transferred));
  }

  void client::receive()
  {
    asio::async_read_until(socket_, reply_buffer_, serverDelim_,
      boost::bind(&client::handle_read, this,
      asio::placeholders::error,
      asio::placeholders::bytes_transferred));

  }

  void client::handle_connect(const asio::error_code& error)
  {
    if (!error)
    {
      handshake();
    }
    else
    {
      //std::cout << "Connect failed: " << error.message() << "\n";
      std::cerr << "Connect failed: " << error.message() << std::endl;
    }
  }

  void client::handle_handshake(const asio::error_code& error)
  {
    if (!error)
    {
      onStart();
    }
    else
    {
      //std::cout << "Handshake failed: " << error.message() << "\n";
      std::cerr << "Handshake failed " << error.message() << std::endl;
    }
  }

  void client::handle_write(const asio::error_code& error,
      size_t bytes_transferred)
  {

    if (!error)
    {
      onWrite();
    }
    else
    {
      //std::cout << "Write failed: " << error.message() << "\n";
      std::cerr << "Write failed " << error.message() << std::endl;
    }
  }

  void client::handle_read(const asio::error_code& error,
      size_t bytes_transferred)
  {

    if (!error)
    {
      bytes_transferred_ = bytes_transferred;
      onRead();
    }
    else
    {
      //std::cout << "Read failed: " << error.message() << "\n";
      std::cerr << "Read failed: " << error.message() << std::endl;
    }
  }