#include "message_handler.hpp"

#include "deserializer.hpp"
#include "serializer.hpp"

MessageHandler::MessageHandler(boost::asio::io_context& io_context)
    : serializer_(std::make_shared<Serializer>()),
      deserializer_(std::make_shared<Deserializer>()),
      io_context_(io_context),
      timer_(io_context_) {
  deserializer_->bell.connect(std::bind(&MessageHandler::OnBell, this,
                                        std::placeholders::_1,
                                        std::placeholders::_2));
  deserializer_->new_object.connect(std::bind(&MessageHandler::OnNewObject,
                                              this, std::placeholders::_1,
                                              std::placeholders::_2));
}

void MessageHandler::OnNewMessage(std::vector<uint8_t> message, int sender_id) {
  deserializer_->ProcessMessage(message, sender_id);
}

void MessageHandler::OnNewObject(int sender_id, int message_id) {
  auto message = serializer_->GetOkMessage(next_message_id_);
  next_message_id_++;
  write_response(message, sender_id);
}

void MessageHandler::OnBell(int sender_id, int message_id) {
  auto message = serializer_->GetOkMessage(message_id);
  write_response(message, sender_id);

  auto someone_at_door = serializer_->GetSomeOneAtDoorMessage(next_message_id_);
  write_response(someone_at_door, 0);
  next_message_id_++;
}

void MessageHandler::OnServiceRequest(int sender_id, int message_id,
                                      std::tuple<bool, bool, bool> status) {
  auto message = serializer_->GetOkMessage(next_message_id_);
  next_message_id_++;
  write_response(message, sender_id);

  auto service_response =
      serializer_->GetServiceMessage(next_message_id_, status);
  write_response(service_response, 0);
  next_message_id_++;
}
