#include "deserializer.hpp"

#include "fbs_generated/request_generated.h"

Deserializer::Deserializer() {}

void Deserializer::ProcessMessage(std::vector<uint8_t> new_message,
                                  int sender_id) {
  auto verifier = flatbuffers::Verifier(new_message.data(), new_message.size());
  if (Communication::VerifyRequestBuffer(verifier)) {
    auto request = Communication::GetRequest(new_message.data());
    auto request_content = request->content_type();
    switch (request_content) {
      case Communication::Content_NONE:
        break;
      case Communication::Content_Bell: {
        auto bell_data = request->content_as_Bell();
        if (bell_data->pressed()) {
          bell(sender_id, request->id());
        };
        break;
      }
      case Communication::Content_Service: {
        auto service_data = request->content_as_Service();
        service_request(sender_id, request->id(),
                        {service_data->camera(), service_data->mic(),
                         service_data->speaker()});
        break;
      }
      case Communication::Content_ObjectDetector: {
        auto object_detector_data = request->content_as_ObjectDetector();
        if (object_detector_data) {
          new_object(sender_id, request->id());
        }
        break;
      }
      case Communication::Content_Session: {
        SessionMetaData metadata;
        auto session_data = request->content_as_Session();
        auto type = session_data->type();
        switch (type) {
          case Communication::SessionType_Controller:
            metadata.type_ = SessionMetaData::Controller;
            break;
          case Communication::SessionType_Follower:
            metadata.type_ = SessionMetaData::Follower;
            break;
          case Communication::SessionType_None:
            metadata.type_ = SessionMetaData::none;
            break;
        }
        metadata.streaming_ip = session_data->streaming_ip()->str();
        session_metadata(sender_id, request->id(), metadata);
      }
    }
  }
}
