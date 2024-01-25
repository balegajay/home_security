#include "session_manager.hpp"

#include <sys/types.h>

#include <algorithm>
#include <boost/system/error_code.hpp>
#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

#include "deserializer.hpp"
#include "serializer.hpp"
#include "session.hpp"

SessionManager::SessionManager(boost::asio::io_context& io_context)
    : io_context_(io_context),
      deserializer_(std::make_shared<Deserializer>(io_context)),
      serializer_(std::make_shared<Serializer>()) {
  // connect to signals from session manager
  message.connect(std::bind(&Deserializer::OnNewMessage, deserializer_.get(),
                            std::placeholders::_1, std::placeholders::_2));

  camera_start_message_ = serializer_->GetCameraStartMessage();
  camera_stop_message_ = serializer_->GetCameraStopMessage();
}

void SessionManager::OnNewObject() { BroadCast(camera_start_message_); }

void SessionManager::AddSession(tcp::socket&& peer) {
  // some hack to get the hash, need to find a better way
  std::shared_ptr<Session> new_session;
  auto session_hash = std::hash<std::shared_ptr<Session>>{}(new_session);
  new_session = Session::Create(std::move(peer), session_hash);
  new_session->new_message.connect(std::bind(&SessionManager::OnRead, this,
                                             std::placeholders::_1,
                                             std::placeholders::_2));
  new_session->session_error.connect(std::bind(&SessionManager::OnSessionError,
                                               this, std::placeholders::_1,
                                               std::placeholders::_2));
  deserializer_->response_ok.connect(std::bind(
      &Session::OnResponseOK, new_session.get(), std::placeholders::_1));
  deserializer_->response_nok.connect(std::bind(
      &Session::OnResponseNOK, new_session.get(), std::placeholders::_1));
  active_sessions_.insert(new_session);
}

void SessionManager::OnRead(std::vector<uint8_t> new_message, int id) {
  message(new_message, id);
}

void SessionManager::OnSessionError(boost::system::error_code err, int id) {
  // TODO handle all errors
  auto it = std::find_if(
      active_sessions_.begin(), active_sessions_.end(),
      [id](std::shared_ptr<Session> session) { return session->id_ == id; });
  if (it != active_sessions_.end()) active_sessions_.erase(it);
}

void SessionManager::BroadCast(std::vector<uint8_t> message) {
  std::for_each(active_sessions_.begin(), active_sessions_.end(),
                [this, &message](std::shared_ptr<Session> session) {
                  session->Write(message);
                });
}