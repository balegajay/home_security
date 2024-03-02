#include "session_manager.hpp"

#include "session.hpp"

SessionManager::SessionManager() {
  // connect to signals from session manager
}

void SessionManager::AddSession(tcp::socket&& peer) {
  // some hack to get the hash, need to find a better way
  std::shared_ptr<Session> new_session;
  auto session_hash = std::hash<std::shared_ptr<Session>>{}(new_session);

  new_session = Session::Create(std::move(peer), session_hash);

  new_session->new_message.connect(std::bind(&SessionManager::OnNewMessage,
                                             this, std::placeholders::_1,
                                             std::placeholders::_2));
  new_session->session_error.connect(std::bind(&SessionManager::OnSessionError,
                                               this, std::placeholders::_1,
                                               std::placeholders::_2));

  active_sessions_[session_hash] = new_session;
}

void SessionManager::OnNewMessage(std::vector<uint8_t> new_message,
                                  int session_id) {
  if (active_sessions_.count(session_id)) message(new_message, session_id);
}

void SessionManager::OnSessionError(boost::system::error_code err, int id) {
  // TODO handle all errors
  session_removed(active_sessions_[id]->metadata_);
  active_sessions_.erase(id);
}

void SessionManager::OnWriteResponse(std::vector<uint8_t> message,
                                     int sender_id) {
  if (sender_id && active_sessions_.count(sender_id)) {
    active_sessions_[sender_id]->WriteResponse(message);
    return;
  }
  // broadcast message is sender id is 0
  for (auto [id, session] : active_sessions_) {
    session->WriteResponse(message);
  }
}
void SessionManager::OnSessionMetaData(int sender_id,
                                       SessionMetaData metadata) {
  if (active_sessions_.count(sender_id)) {
    session_metadata_changed(active_sessions_[sender_id]->metadata_, metadata);
    active_sessions_[sender_id]->metadata_ = metadata;
  }
}
