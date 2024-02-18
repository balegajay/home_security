#include "serializer.hpp"

#include "fbs_generated/request_generated.h"
#include "fbs_generated/response_generated.h"

Serializer::Serializer() {}

std::vector<uint8_t> Serializer::GetServiceMessage(
    int message_id, std::tuple<bool, bool, bool> status) {
  flatbuffers::FlatBufferBuilder builder(1024);
  auto camera_status =
      Communication::Service((Communication::Status)std::get<0>(status),
                             (Communication::Status)std::get<1>(status),
                             (Communication::Status)std::get<2>(status));
  auto offset = builder.CreateStruct(camera_status);
  auto request = Communication::CreateRequest(
      builder, message_id, Communication::Content_Service, offset.Union());
  Communication::FinishRequestBuffer(builder, request);
  return {builder.GetBufferPointer(),
          (builder.GetBufferPointer() + builder.GetSize())};
}

std::vector<uint8_t> Serializer::GetOkMessage(int message_id) {
  flatbuffers::FlatBufferBuilder builder(1024);
  auto ok_code = Communication::ResponseCode(Communication::Code_OK);
  auto offset = builder.CreateStruct(ok_code);
  auto response = Communication::CreateResponse(
      builder, message_id, Communication::ResponseContent_ResponseCode,
      offset.Union());
  Communication::FinishResponseBuffer(builder, response);
  return {builder.GetBufferPointer(),
          builder.GetBufferPointer() + builder.GetSize()};
}

std::vector<uint8_t> Serializer::GetErrorMessage(int message_id) {
  flatbuffers::FlatBufferBuilder builder(1024);
  auto ok_code = Communication::ResponseCode(Communication::Code_ERROR);
  auto offset = builder.CreateStruct(ok_code);
  auto response = Communication::CreateResponse(
      builder, message_id, Communication::ResponseContent_ResponseCode,
      offset.Union());
  Communication::FinishResponseBuffer(builder, response);
  return {builder.GetBufferPointer(),
          builder.GetBufferPointer() + builder.GetSize()};
}

std::vector<uint8_t> Serializer::GetSomeOneAtDoorMessage(int message_id) {
  flatbuffers::FlatBufferBuilder builder(1024);
  auto offset = builder.CreateStruct(Communication::SomeOneAtDoor(true));
  auto response = Communication::CreateResponse(
      builder, message_id, Communication::ResponseContent_SomeOneAtDoor,
      offset.Union());
  Communication::FinishResponseBuffer(builder, response);
  return {builder.GetBufferPointer(),
          builder.GetBufferPointer() + builder.GetSize()};
}
