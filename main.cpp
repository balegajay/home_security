#include <functional>
#include <iostream>
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <string>

#include "IInference.hpp"
#include "streamer.hpp"
#include "torchInference.hpp"

int main(int argc, char *argv[]) {
  std::function<cv::Mat()> cb;
  Streamer streamer(argc, argv, 5000, 1, cb);
  streamer.Setup();
  streamer.Start();
  // std::unique_ptr<IInference> inferencer =
  //   std::make_unique<TorchInference>("", "");
  return 0;
}