#include "s_talkative_bunch.h"


  talkative_bunch::talkative_bunch(asio::io_service& io_service, asio::ssl::context& context,
    std::string host, std::string port,
    std::string clientDelim, std::string serverDelim):
      client::client(io_service, context, host, port,
      clientDelim, serverDelim)
  {
    local_counter = 0;
  }


  std::string talkative_bunch::getMessage()
  {
    return ("<com><data>" + std::to_string(local_counter) + "</data></com>");
  }

  std::string talkative_bunch::convertData(int reply_length)
  {

    asio::streambuf::const_buffers_type bufstream = reply_buffer_.data();
    std::string response(asio::buffers_begin(bufstream), asio::buffers_begin(bufstream) + reply_length);
    // Vyčištění bufferu.
    reply_buffer_.consume(reply_length);
    return response;
  }

 void talkative_bunch::onStart()
  {

    std::string msg = "hello" + clientDelim_;
    send(msg);
  }

  void talkative_bunch::onWrite()
  {

    receive();
  }

  void talkative_bunch::onRead()
  {
    reply_ = convertData(bytes_transferred_);
    std::cout << "Reply: " << reply_ << std::endl;
    local_counter++;

    if (local_counter == 5)
    {
      return;
    }
    std::string msg = getMessage();
    send(msg);
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
  talkative_bunch c(io_service, ctx, host, port, clientDelim, serverDelim);

  c.connect();
  //c.send("sprava");
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

    //    makeClient(clientDelim, serverDelim, host, port, certpath);
    
    for (int i = 0; i < 100; ++i)
    {
      try{
        makeClient(clientDelim, serverDelim, host, port, certpath);
      }
      catch (asio::system_error const& e)
      {
        std::cerr << "Client exception: " << e.what() << std::endl;
      }

    }
    
 
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
