#pragma once
#include <torch/script.h>
#include <torch/torch.h>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <string>

#include "IInference.hpp"

class TorchInference : public IInference {
 private:
  torch::Device device_;
  std::vector<std::string> classes_;
  torch::jit::script::Module yolo_model_;

 public:
  TorchInference(fs::path model_path, fs::path class_file);
  virtual std::tuple<cv::Mat, std::string, float> Infer(cv::Mat in_image);
};
