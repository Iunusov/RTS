#pragma once
#include <tuple>

#include "Math.hpp"

struct Coord final {
  using T = long double;
  T x{0};
  T y{0};
  T z{0};

  Coord(T x, T y, T z = 0) noexcept : x(x), y(y), z(z) {}
  Coord() noexcept = default;
  Coord(const Coord &) noexcept = default;
  Coord(Coord &&) noexcept = default;

  constexpr bool operator<=(const Coord &other) const noexcept {
    return (x < other.x) || (y < other.y) || (z < other.z) || (*this == other);
  }

  constexpr Coord &operator=(const Coord &) noexcept = default;

  constexpr decltype(auto) distance(const Coord &other) const noexcept {
    return Math::dist(*this, other);
  }

  constexpr bool operator==(const Coord &r) const noexcept {
    return (std::tie(x, y, z) == std::tie(r.x, r.y, r.z));
  }
};

inline Coord operator+(const Coord &l, const Coord &r) noexcept {
  return Coord{l.x + r.x, l.y + r.y, l.z + r.z};
}

inline Coord operator-(const Coord &l, const Coord &r) noexcept {
  // if(r.x>=l.x ||r.y>=l.y ||r.z>=l.z){exit(1);}
  return Coord{l.x - r.x, l.y - r.y, l.z - r.z};
}
