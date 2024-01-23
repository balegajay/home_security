#pragma once

#include <functional>
#include <memory>
#include <opencv2/core.hpp>
#include <boost/signals2/signal.hpp>

class Impl;

class Streamer {
  Impl* impl_data_;
  int video_port_;
  int audio_port_;

 public:
  Streamer(int argc, char* argv[], int video_port, int audio_port);
  ~Streamer();
  bool Setup();
  bool Pause();
  void Start();
  boost::signals2::signal<void(cv::Mat)> new_img;
};
