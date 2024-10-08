#pragma once

struct Coord final {
  using T = double;
  T x{0};
  T y{0};

  constexpr Coord(T x_, T y_) noexcept : x(x_), y(y_) {}

  constexpr Coord() noexcept = default;
  constexpr Coord(const Coord &) noexcept = default;
  constexpr Coord(Coord &&) noexcept = default;
  constexpr Coord &operator=(const Coord &) noexcept = default;

  constexpr bool operator==(const Coord &r) const noexcept {
    return x == r.x && y == r.y;
  }

  constexpr Coord &operator+=(const Coord &r) noexcept {
    x += r.x;
    y += r.y;
    return *this;
  }

  constexpr Coord &operator-=(const Coord &r) noexcept {
    x -= r.x;
    y -= r.y;
    return *this;
  }
};
