#pragma once
#include <boost/asio.hpp>
#include <boost/signals2.hpp>
#include <iostream>
#include <memory>
#include <vector>

class Deserializer;
class Serializer;

class MessageHandler {
 private:
  boost::asio::io_context& io_context_;
  std::shared_ptr<Deserializer> deserializer_;
  std::shared_ptr<Serializer> serializer_;
  int next_message_id_ = 0;
  boost::asio::steady_timer timer_;

 public:
  MessageHandler(boost::asio::io_context& io_context);
  void OnNewMessage(std::vector<uint8_t> message, int sender_id);
  void OnNewObject(int sender_id, int message_id);
  void OnBell(int sender_id, int message_id);
  boost::signals2::signal<void(int, bool)> change_camera_status;
  boost::signals2::signal<void(int, bool)> change_mic_status;
  boost::signals2::signal<void(int, bool)> change_speaker_status;
  void OnServiceRequest(int sender_id, int message_id,
                        std::tuple<bool, bool, bool> status);
  boost::signals2::signal<void(std::vector<uint8_t> message, int sender_id)>
      write_response;
};