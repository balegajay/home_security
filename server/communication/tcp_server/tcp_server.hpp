#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/signals2.hpp>
#include <iostream>

#include "session_manager.hpp"

using boost::asio::ip::tcp;

class TCPServer {
  TCPServer(uint16_t port);
  ~TCPServer();
  void StartAsync();

 private:
  void StartAccept();
  boost::asio::io_context io_context_;
  tcp::acceptor acceptor_;
  bool running_ = false;
  SessionManager session_manager_;
};