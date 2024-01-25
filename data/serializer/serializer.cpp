#include "serializer.hpp"

#include "fbs_generated/message_generated.h"

Serializer::Serializer() {}

std::vector<uint8_t> Serializer::GetCameraStartMessage() {
  flatbuffers::FlatBufferBuilder builder(1024);
  auto camera_status = Communication::Camera(Communication::Status_Start);
  auto offset = builder.CreateStruct(camera_status);
  auto message = Communication::CreateMessage(
      builder, Communication::Content_camera, offset.Union());
  Communication::FinishMessageBuffer(builder, message);
  return {builder.GetBufferPointer(),
          (builder.GetBufferPointer() + builder.GetSize())};
}

std::vector<uint8_t> Serializer::GetCameraStopMessage() {
  flatbuffers::FlatBufferBuilder builder(1024);
  auto camera_status = Communication::Camera(Communication::Status_Stop);
  auto offset = builder.CreateStruct(camera_status);
  auto message = Communication::CreateMessage(
      builder, Communication::Content_camera, offset.Union());
  Communication::FinishMessageBuffer(builder, message);
  return {builder.GetBufferPointer(),
          (builder.GetBufferPointer() + builder.GetSize())};
}