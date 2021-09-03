#pragma once

#include <cstdint>

struct Coord final {
  long double x{0};
  long double y{0};
  long double z{0};

  Coord(long double x, long double y, long double z = 0) noexcept
      : x(x), y(y), z(z) {}
  Coord() noexcept = default;
};
