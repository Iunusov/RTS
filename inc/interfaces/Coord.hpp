#pragma once

#include <cmath>
#include <cstdint>

struct Coord final {
  long double x{0};
  long double y{0};
  long double z{0};

  Coord(long double x, long double y, long double z = 0) noexcept
      : x(x), y(y), z(z) {}
  Coord() noexcept = default;

  size_t distance(const Coord &other) const noexcept {
    return (size_t)sqrt(pow(other.x - x, 2) + pow(other.y - y, 2));
  }

  bool operator==(const Coord &other) const noexcept {
    return ((x == other.x) && (y == other.y) && (z == other.z));
  }
};
