#include "deserializer.hpp"

#include <iostream>

#include "fbs_generated/message_generated.h"

Deserializer::Deserializer() {}

void Deserializer::ProcessMessage(std::vector<uint8_t> new_message) {
  auto verifier = flatbuffers::Verifier(new_message.data(), new_message.size());

  if (Communication::VerifyMessageBuffer(verifier)) {
    auto message = Communication::GetMessage(new_message.data());

    switch (message->content_type()) {
      case Communication::Content_NONE: {
        std::cout << "error with message content type" << std::endl;
        break;
      }
      case Communication::Content_Notification: {
        auto notification = message->content_as_Notification();
        new_notification(message->id(), static_cast<Notification>(
                                            notification->notification()));
        break;
      }
      case Communication::Content_Request: {
        auto request = message->content_as_Request();
        if (request->audio_feed() && request->video_feed())
          new_request(message->id(), AVFeed);
        else if (request->audio_feed())
          new_request(message->id(), AudioFeed);
        else if (request->video_feed())
          new_request(message->id(), VideoFeed);
        break;
      }
      case Communication::Content_Response: {
        break;
      }
      case Communication::Content_Heartbeat: {
        heartbeat(message->id());
        break;
      }
    }
  }
}
