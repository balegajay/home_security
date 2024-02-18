#pragma once
#include <iostream>
#include <vector>

class Serializer {
 public:
  Serializer();
  std::vector<uint8_t> GetOkMessage(int id);
  std::vector<uint8_t> GetErrorMessage(int id);
  std::vector<uint8_t> GetSomeOneAtDoorMessage(int id);

  std::vector<uint8_t> GetServiceMessage(int id,
                                         std::tuple<bool, bool, bool> status);
};