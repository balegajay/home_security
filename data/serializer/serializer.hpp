#pragma once
#include <iostream>
#include <vector>
#include <cstdint>

class Serializer {
 public:
  Serializer();
  std::vector<uint8_t> GetOkMessage(int id);
};