#pragma once
#include <cmath>
#include <numbers>

namespace Math {
constexpr const double rad = std::numbers::pi / 180.0;

constexpr double P2(const double a, const double b) noexcept {
  const double tmp = a - b;
  return tmp * tmp;
}

template <class T> constexpr double dist(const T &a, const T &b) noexcept {
  return P2(a.x, b.x) + P2(a.y, b.y);
}

template <class T>
constexpr T move(const double len, const double heading) noexcept {
  return T{len * cos(heading * rad), len * sin(heading * rad)};
}

constexpr double CalculateScreenPosition(const double obj_coord,
                                         const double obj_dimension,
                                         const double camera_coord,
                                         const double screen_resolution,
                                         const double zoom_value) noexcept {
  return obj_coord - obj_dimension / 2.0 - camera_coord +
         (screen_resolution / 2.0) / zoom_value;
}

} // namespace Math
