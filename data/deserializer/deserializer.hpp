#pragma once

#include <boost/asio.hpp>
#include <boost/signals2.hpp>
#include <tuple>
#include <vector>

#include "session_type.hpp"

class Deserializer {
 public:
  Deserializer();
  void ProcessMessage(std::vector<uint8_t> new_message, int id);
  boost::signals2::signal<void(int sender_id, int message_id)> new_object;
  boost::signals2::signal<void(int sender_id, int message_id)> bell;
  boost::signals2::signal<void(int sender_id, int message_id,
                               std::tuple<bool, bool, bool>)>
      service_request;
  boost::signals2::signal<void(int sender_id, int message_id, SessionType type)>
      session_type;
};