#include "session.hpp"

Session::Session(tcp::socket &&peer, int id)
    : peer_(std::move(peer)), id_(id) {}

std::shared_ptr<Session> Session::Create(tcp::socket &&peer, int id) {
  return std::make_shared<Session>(std::move(peer), id);
}

void Session::Read() {
  peer_.async_receive(boost::asio::buffer(buffer_, buffer_.size()),
                      [this](boost::system::error_code err, size_t len) {
                        if (!err) {
                          new_message(buffer_, id_);
                          Read();
                        } else {
                          session_error(err, id_);
                        }
                      });
}
void Session::Write(std::vector<uint8_t> message) {
  if (message.size()) {
    peer_.async_write_some(boost::asio::buffer(message.data(), message.size()),
                           [this](boost::system::error_code err, size_t len) {
                             if (err) {
                               session_error(err, id_);
                             }
                           });
  }
}