#include "s_single.h"


  s_single::s_single(asio::io_service& io_service, asio::ssl::context& context,
    std::string host, std::string port,
    std::string clientDelim, std::string serverDelim):
      client::client(io_service, context, host, port,
      clientDelim, serverDelim)
  {
    local_counter = 0;

  }



  std::string s_single::getMessage()
  {
    return ("<com><data>" + std::to_string(local_counter) + "</data></com>");
  }

  std::string s_single::convertData(int reply_length)
  {
    asio::streambuf::const_buffers_type bufstream = reply_buffer_.data();
    std::string response(asio::buffers_begin(bufstream), asio::buffers_begin(bufstream) + reply_length);
    // Vyčištění bufferu.
    reply_buffer_.consume(reply_length);
    return response;
  }

 void s_single::onStart()
  {
    std::string msg = "hello" + clientDelim_;
    send(msg);
  }

  void s_single::onWrite()
  {

    receive();
  }

  void s_single::onRead()
  {
    reply_ = convertData(bytes_transferred_);
    std::cout << "Reply: " << reply_ << std::endl;
    return;
  }

asio::ssl::context prepareContext(asio::io_service& io, std::string certpath)
{
  asio::ssl::context ctx(io, asio::ssl::context::sslv23);
  ctx.set_verify_mode(asio::ssl::context::verify_peer);
  ctx.load_verify_file(certpath);
  return ctx;
}

void makeClient(std::string clientDelim, std::string serverDelim,
    std::string host, std::string port, std::string certpath)
{
  asio::io_service io_service;
  asio::ssl::context ctx = prepareContext(io_service, certpath);
  s_single c(io_service, ctx, host, port, clientDelim, serverDelim);

  c.connect();
  io_service.run();
}

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: client <host> <port>\n";
      return 1;
    }

    std::string clientDelim("</com>");
    std::string serverDelim("</end>");    
    std::string host = argv[1];
    std::string port = argv[2];
    std::string certpath("cert/client.crt");

    makeClient(clientDelim, serverDelim, host, port, certpath);
 
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
