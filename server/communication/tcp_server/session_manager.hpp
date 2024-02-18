#include <boost/asio.hpp>
#include <boost/signals2.hpp>
#include <boost/system/error_code.hpp>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

#include "session.hpp"
#include "session_type.hpp"

class SessionManager {
 private:
  std::unordered_map<int, std::shared_ptr<Session>> active_sessions_;
  int new_session_id_ = 0;

 public:
  SessionManager();
  void AddSession(tcp::socket&& peer);
  void OnNewMessage(std::vector<uint8_t> message, int session_id);
  void OnSessionError(boost::system::error_code err, int id);
  void BroadCast(std::vector<uint8_t> message);
  void OnWriteResponse(std::vector<uint8_t> message, int sender_id);
  void OnSessionType(int sender_id, SessionType type);
  boost::signals2::signal<void(std::vector<uint8_t>, int)> message;
};