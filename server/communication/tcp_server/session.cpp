#include "session.hpp"

#include "fbs_generated/message_generated.h"

void Session::ReadHandler(std::vector<uint8_t> data) {
  deserializer_.ProcessMessage(data);
}

Session::Session(tcp::socket &&peer, int id) : peer_(std::move(peer)), id_(id) {
  deserializer_.new_notification.connect(std::bind(&Session::onNotification,
                                                   this, std::placeholders::_1,
                                                   std::placeholders::_2));
  deserializer_.heartbeat.connect(
      std::bind(&Session::onHeartBeat, this, std::placeholders::_1));
}

std::shared_ptr<Session> Session::Create(tcp::socket &&peer, int id) {
  return std::make_shared<Session>(std::move(peer), id);
}

void Session::Read() {
  boost::asio::async_read(
      peer_, boost::asio::buffer(buffer_, sizeof(Communication::Message)),
      [this](boost::system::error_code err, size_t len) {
        if (!err) {
          ReadHandler(buffer_);
          Read();
        } else {
          peer_.close();
          session_error(err, id_);
        }
      });
}

void Session::WriteResponse(std::vector<uint8_t> message) {
  if (message.size()) {
    peer_.async_write_some(boost::asio::buffer(message.data(), message.size()),
                           [this](boost::system::error_code err, size_t len) {
                             if (err) {
                               peer_.close();
                               session_error(err, id_);
                             }
                           });
  }
}

void Session::onHeartBeat(int request_id) {
  auto message = serializer_.GetOkMessage(request_id);
  WriteResponse(message);
}

void Session::onNotification(int request_id, Notification notification) {
  auto message = serializer_.GetOkMessage(request_id);
  WriteResponse(message);
  new_notification(notification);
}
void Session::onRequest(int request_id, Request request) {
  auto message = serializer_.GetOkMessage(request_id);
  WriteResponse(message);
  new_request(request);
}
