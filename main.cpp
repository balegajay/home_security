#include <iostream>
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <string>

#include "IInference.hpp"
#include "torchInference.hpp"

int main() {
  std::unique_ptr<IInference> inferencer =
      std::make_unique<TorchInference>("", "");
  return 0;
}
