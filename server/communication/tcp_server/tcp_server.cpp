#include "tcp_server.hpp"

#include <algorithm>
#include <memory>
#include <vector>

TCPServer::TCPServer(uint16_t port)
    : io_context_(), acceptor_(io_context_, tcp::endpoint(tcp::v4(), port)) {}

TCPServer::~TCPServer() {}

void TCPServer::StartAsync() {
  if (running_) return;
  StartAccept();
  running_ = true;
}

void TCPServer::StartAccept() {
  acceptor_.async_accept(
      io_context_,
      [this](const boost::system::error_code &err, tcp::socket peer) {
        if (!err) {
          session_manager_.AddSession(std::move(peer));
        }
      });
}