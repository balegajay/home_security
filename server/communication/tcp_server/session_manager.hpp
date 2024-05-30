#include <boost/asio.hpp>
#include <boost/signals2.hpp>
#include <boost/system/error_code.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

#include "notifications.hpp"

using boost::asio::ip::tcp;
class Session;

class SessionManager {
 private:
  std::unordered_map<int, std::shared_ptr<Session>> active_sessions_;
  int new_session_id_ = 0;

 public:
  SessionManager();
  void OnNewSession(tcp::socket&& peer);
  boost::signals2::signal<void(Notification)> new_notification;
  void OnSessionError(boost::system::error_code err, int id);
  void BroadCast(std::vector<uint8_t> message);
  void OnWriteResponse(std::vector<uint8_t> message, int sender_id);
};