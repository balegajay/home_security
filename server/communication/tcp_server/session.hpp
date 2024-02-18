#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/signals2.hpp>
#include <unordered_set>

using boost::asio::ip::tcp;

class Session : std::enable_shared_from_this<Session> {
  tcp::socket peer_;
  std::vector<uint8_t> buffer_;

 public:
  Session(tcp::socket &&peer, int id);
  int id_;
  [[nodiscard]] static std::shared_ptr<Session> Create(tcp::socket &&peer,
                                                       int id);
  void Read();
  void WriteResponse(std::vector<uint8_t> message);
  boost::signals2::signal<void(std::vector<uint8_t>, int)> new_message;
  boost::signals2::signal<void(boost::system::error_code, int)> session_error;
};