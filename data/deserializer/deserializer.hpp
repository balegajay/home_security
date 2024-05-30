#pragma once

#include <boost/asio.hpp>
#include <boost/signals2.hpp>
#include <tuple>
#include <vector>

#include "notifications.hpp"
#include "request.hpp"

class Deserializer {
 public:
  Deserializer();
  void ProcessMessage(std::vector<uint8_t> new_message);
  boost::signals2::signal<void(int, Notification)> new_notification;
  boost::signals2::signal<void(int)> heartbeat;
  boost::signals2::signal<void(int, Request)> new_request;
};