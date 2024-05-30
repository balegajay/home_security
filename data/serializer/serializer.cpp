#include "serializer.hpp"

#include "fbs_generated/message_generated.h"

Serializer::Serializer() {}

std::vector<uint8_t> Serializer::GetOkMessage(int message_id) {
  flatbuffers::FlatBufferBuilder builder(1024);
  auto ok_code = Communication::Response(true);
  auto offset = builder.CreateStruct(ok_code);
  auto response = Communication::CreateMessage(
      builder, message_id, Communication::Content_Response, offset.Union());
  Communication::FinishMessageBuffer(builder, response);
  return {builder.GetBufferPointer(),
          builder.GetBufferPointer() + builder.GetSize()};
}
