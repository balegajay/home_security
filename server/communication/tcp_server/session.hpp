#pragma once

#include <boost/asio.hpp>
#include <boost/signals2.hpp>

#include "deserializer.hpp"
#include "notifications.hpp"
#include "serializer.hpp"

using boost::asio::ip::tcp;

class Session : std::enable_shared_from_this<Session> {
  tcp::socket peer_;
  std::vector<uint8_t> buffer_;
  Deserializer deserializer_;
  Serializer serializer_;
  void ReadHandler(std::vector<uint8_t> data);

 public:
  Session(tcp::socket &&peer, int id);
  int id_;
  [[nodiscard]] static std::shared_ptr<Session> Create(tcp::socket &&peer,
                                                       int id);
  void Read();
  void WriteResponse(std::vector<uint8_t> message);
  boost::signals2::signal<void(Notification)> new_notification;
  boost::signals2::signal<void(Request)> new_request;

  boost::signals2::signal<void(boost::system::error_code, int)> session_error;
  void onHeartBeat(int request_id);
  void onNotification(int request_id, Notification notification);
  void onRequest(int request_id, Request request);
};