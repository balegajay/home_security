#include <boost/signals2.hpp>
#include <boost/system/error_code.hpp>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

#include "session.hpp"

class SessionManager {
 private:
  std::unordered_map<int, std::shared_ptr<Session>> active_sessions_;
  std::unordered_map<int, int> current_requests_;
  int new_session_id_ = 0;
  int new_request_id_ = 0;

 public:
  SessionManager();
  void AddSession(tcp::socket&& peer);
  void OnRead(std::vector<uint8_t> message, int id);
  void OnSessionError(boost::system::error_code err, int id);
  void BroadCast(std::vector<uint8_t> message);
  boost::signals2::signal<void(std::vector<uint8_t>)> message;
};