#ifndef SESSION_TYPE_HPP
#define SESSION_TYPE_HPP
#include <string>

struct SessionMetaData {
  enum SessionType { none, Controller, Follower } type_;
  std::string streaming_ip;
};
#endif  // SESSION_TYPE_HPP
