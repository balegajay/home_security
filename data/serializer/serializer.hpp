#pragma once
#include <iostream>
#include <vector>

class Serializer {
 public:
  Serializer();
  std::vector<uint8_t> GetCameraStartMessage();
  std::vector<uint8_t> GetCameraStopMessage();
};