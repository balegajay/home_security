#include "tcp_server.hpp"

#include <iostream>

TCPServer::TCPServer(uint16_t port, boost::asio::io_context& io_context)
    : io_context_(io_context),
      acceptor_(io_context_, tcp::endpoint(tcp::v4(), port)) {
  session_manager_.new_notification.connect([](Notification notification) {
    std::cout << "Received notification: " << notification << std::endl;
  });
}

TCPServer::~TCPServer() {}

void TCPServer::StartAsync() {
  if (running_) return;
  StartAccept();
  running_ = true;
}

void TCPServer::StartAccept() {
  acceptor_.async_accept(
      io_context_,
      [this](const boost::system::error_code& err, tcp::socket peer) {
        if (!err) {
          session_manager_.OnNewSession(std::move(peer));
        }
      });
}