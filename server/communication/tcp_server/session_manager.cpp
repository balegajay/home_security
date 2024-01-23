#include "session_manager.hpp"

#include <sys/types.h>

#include <algorithm>
#include <boost/system/error_code.hpp>
#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

#include "Session.hpp"

SessionManager::SessionManager() {}

void SessionManager::AddSession(tcp::socket&& peer) {
  while (active_sessions_.count(new_session_id_) != 0) {
    new_session_id_++;
  }
  auto new_session = Session::Create(std::move(peer), new_session_id_);
  new_session->new_message.connect(std::bind(&SessionManager::OnRead, this,
                                             std::placeholders::_1,
                                             std::placeholders::_2));
  new_session->session_error.connect(std::bind(&SessionManager::OnSessionError,
                                               this, std::placeholders::_1,
                                               std::placeholders::_2));
  active_sessions_[new_session_id_] = new_session;
}

void SessionManager::OnRead(std::vector<uint8_t> new_message, int id) {
  // TODO add deserialisation here
  message(new_message);
}

void SessionManager::OnSessionError(boost::system::error_code err, int id) {
  // TODO handle all errors
  active_sessions_.erase(id);
}

void SessionManager::BroadCast(std::vector<uint8_t> message) {
  std::for_each(active_sessions_.begin(), active_sessions_.end(),
                [this, &message](std::shared_ptr<Session> session) {
                  while (current_requests_.count(new_request_id_) != 0) {
                    new_request_id_++;
                  }
                  current_requests_[new_request_id_] = session->id_;
                  session->Write(message);
                });
}