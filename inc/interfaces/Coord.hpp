#pragma once

#include <cstdint>

struct Coord final {
  int64_t x{0};
  int64_t y{0};
  int64_t z{0};

  Coord(int64_t x, int64_t y, int64_t z = 0) noexcept : x(x), y(y), z(z) {}
  Coord() noexcept = default;
};
