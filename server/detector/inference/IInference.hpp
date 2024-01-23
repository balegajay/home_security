#pragma once
#include <filesystem>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <tuple>

namespace fs = std::filesystem;

class IInference {
 public:
  IInference(fs::path model_path, fs::path class_file)
      : model_path_(model_path), class_file_(class_file){};
  ~IInference() = default;
  virtual std::tuple<cv::Mat, std::string, float> Infer(cv::Mat in_image) = 0;
  fs::path model_path_;
  fs::path class_file_;
};