#include <boost/asio.hpp>
#include <boost/signals2.hpp>
#include <boost/system/error_code.hpp>
#include <cstdint>
#include <memory>
#include <unordered_set>
#include <vector>

#include "session.hpp"

class Deserializer;
class Serializer;

class SessionManager {
 private:
  std::unordered_set<std::shared_ptr<Session>> active_sessions_;
  int new_session_id_ = 0;
  int new_request_id_ = 0;
  std::shared_ptr<Deserializer> deserializer_;
  std::shared_ptr<Serializer> serializer_;
  boost::asio::io_context& io_context_;
  std::vector<uint8_t> camera_start_message_;
  std::vector<uint8_t> camera_stop_message_;

 public:
  SessionManager(boost::asio::io_context& io_context);
  void AddSession(tcp::socket&& peer);
  void OnRead(std::vector<uint8_t> message, int id);
  void OnSessionError(boost::system::error_code err, int id);
  void BroadCast(std::vector<uint8_t> message);
  boost::signals2::signal<void(std::vector<uint8_t>, int)> message;
  void OnNewObject();
};