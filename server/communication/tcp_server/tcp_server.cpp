#include "tcp_server.hpp"

TCPServer::TCPServer(uint16_t port)
    : io_context_(),
      acceptor_(io_context_, tcp::endpoint(tcp::v4(), port)),
      session_manager_(),
      message_handler_(io_context_) {
  session_manager_.message.connect(
      std::bind(&MessageHandler::OnNewMessage, &message_handler_,
                std::placeholders::_1, std::placeholders::_2));
  message_handler_.write_response.connect(
      std::bind(&SessionManager::OnWriteResponse, &session_manager_,
                std::placeholders::_1, std::placeholders::_2));
  message_handler_.session_metadata.connect(
      std::bind(&SessionManager::OnSessionMetaData, &session_manager_,
                std::placeholders::_1, std::placeholders::_2));
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
      [this](const boost::system::error_code &err, tcp::socket peer) {
        if (!err) {
          session_manager_.AddSession(std::move(peer));
        }
      });
}