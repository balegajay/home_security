#include "session_manager.hpp"

#include <iostream>

#include "session.hpp"

SessionManager::SessionManager() {
  // connect to signals from session manager
}

void SessionManager::OnNewSession(tcp::socket&& peer) {
  // some hack to get the hash, need to find a better way
  std::shared_ptr<Session> new_session;
  auto session_hash = std::hash<std::shared_ptr<Session>>{}(new_session);

  new_session = Session::Create(std::move(peer), session_hash);

  new_session->new_notification.connect(this->new_notification);

  new_session->session_error.connect(std::bind(&SessionManager::OnSessionError,
                                               this, std::placeholders::_1,
                                               std::placeholders::_2));

  active_sessions_[session_hash] = new_session;
}

void SessionManager::OnSessionError(boost::system::error_code err, int id) {
  std::cout << "removing client: " << id << " due to err: " << err << std::endl;
  active_sessions_.erase(id);
}