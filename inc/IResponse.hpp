#pragma once

#include <string>

class IResponse final {
public:
  std::string get_string{"N/A"};
  bool is_ready{false};
  int64_t get_int{-1};

  IResponse(int64_t val) noexcept {
    is_ready = true;
    get_int = val;
    get_string = std::to_string(val);
  }
  IResponse(size_t val) noexcept : IResponse((int64_t)(val)) {}

  IResponse(const char *str) noexcept {
    is_ready = true;
    get_string = str;
    get_int = get_string.size();
  }
  IResponse() = default;
  IResponse(const IResponse &) noexcept = default;
  IResponse(IResponse &&) noexcept = default;
};
