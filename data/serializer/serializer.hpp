#pragma once
#include <iostream>
#include <vector>
#include <cstdint>

class Serializer {
 public:
  Serializer();
  std::vector<uint8_t> GetCameraStartMessage();
  std::vector<uint8_t> GetCameraStopMessage();
};