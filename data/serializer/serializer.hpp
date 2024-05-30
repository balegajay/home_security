#pragma once
#include <iostream>
#include <vector>

class Serializer {
 public:
  Serializer();
  std::vector<uint8_t> GetOkMessage(int id);
};