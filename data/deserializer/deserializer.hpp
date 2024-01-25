#pragma once

#include <boost/asio.hpp>
#include <boost/signals2.hpp>
#include <iostream>
#include <vector>

class Deserializer {
  boost::asio::io_context& io_context_;
  void ProcessMessage(std::vector<uint8_t> new_message, int id);

 public:
  Deserializer(boost::asio::io_context& io_context);
  void OnNewMessage(std::vector<uint8_t> new_message, int id);
  boost::signals2::signal<void()> new_object;
  boost::signals2::signal<void(int id)> response_ok;
  boost::signals2::signal<void(int id)> response_nok;
};