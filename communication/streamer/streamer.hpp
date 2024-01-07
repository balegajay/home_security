#pragma once

#include <functional>
#include <memory>
#include <opencv2/core.hpp>

class Impl;

class Streamer {
  Impl* impl_data_;
  std::function<cv::Mat()> call_back_;
  int video_port_;
  int audio_port_;

 public:
  Streamer(int argc, char* argv[], int video_port, int audio_port,
           std::function<cv::Mat()> call_back);
  ~Streamer();
  bool Setup();
  bool is_playing();
  bool Play();
  bool Pause();
  void Start();
};
