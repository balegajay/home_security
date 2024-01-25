#include "deserializer.hpp"

#include "fbs_generated/message_generated.h"

Deserializer::Deserializer(boost::asio::io_context& io_context)
    : io_context_(io_context) {}

void Deserializer::ProcessMessage(std::vector<uint8_t> new_message, int id) {
  auto verifier = flatbuffers::Verifier(new_message.data(), new_message.size());
  if (Communication::VerifyMessageBuffer(verifier)) {
    auto message = Communication::GetMessage(new_message.data());
    auto content_type = message->content_type();
    switch (content_type) {
      case Communication::Content_NONE:
        break;
      case Communication::Content_sensor: {
        auto sensor_data = message->content_as_sensor();
        if (sensor_data->new_object()) new_object();
        break;
      }
      case Communication::Content_camera: {
        break;
      }
      case Communication::Content_response: {
        auto response_content = message->content_as_response();
        if (response_content->value() == Communication::ResponseContent_OK)
          response_ok(id);
        else
          response_nok(id);
      }
    }
  }
}

void Deserializer::OnNewMessage(std::vector<uint8_t> new_message, int id) {
  boost::asio::post(io_context_, [this, new_message, id]() {
    this->ProcessMessage(new_message, id);
  });
}
