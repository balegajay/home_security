#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/signals2.hpp>

#include "session_manager.hpp"
using boost::asio::ip::tcp;

class TCPServer {
  boost::asio::io_context& io_context_;
  SessionManager session_manager_;
  tcp::acceptor acceptor_;
  void StartAccept();
  bool running_ = false;

 public:
  TCPServer(uint16_t port, boost::asio::io_context& io_context);
  ~TCPServer();
  void StartAsync();
};