#pragma once
#include "CppHacks.hpp"
#include <cmath>

namespace Math {
constexpr double PI_Constant{3.14159265358979323846};

template <class T> INLINE constexpr T P2(const T a, const T b) noexcept {
  const T tmp{(a > b) ? a - b : b - a};
  return (tmp * tmp);
}

template <class T>
INLINE constexpr decltype(auto) dist(const T &a, const T &b) noexcept {
  return P2(a.x, b.x) + P2(a.y, b.y);
}

template <class T, class D>
INLINE constexpr T move(const D len, const double heading) noexcept {
  constexpr const auto rad{PI_Constant / 180.0};
  return T{(decltype(T::x))len * cos(heading * rad),
           (decltype(T::x))len * sin(heading * rad)};
}

template <class T>
INLINE constexpr T CalculateScreenPosition(const T obj_pos, const T camPos,
                                           const T screen_resolution,
                                           const T scale) {
  return (obj_pos - camPos + (screen_resolution / scale) / 2);
}

} // namespace Math
