#pragma once

#include <functional>
#include <memory>
#include <boost/signals2/signal.hpp>

class Impl;

class Streamer {
  Impl* impl_data_;

 public:
  Streamer(int argc, char* argv[]);
  ~Streamer();
  bool Setup(std::string pipeline);
  bool Pause();
  void Start();
};
