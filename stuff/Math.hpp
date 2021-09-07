#pragma once
#include <cmath>

namespace Math {
constexpr long double PI_Constant{3.14159265358979323846};

template <class T>
constexpr T lerp(const T a, const T b, const long double t) noexcept {
  return (T)(a + t * (b - a));
}

template <class T>
constexpr T P2(const T a, const T b) noexcept {
  const decltype(a) tmp{(a > b) ? a - b : b - a};
  return (tmp * tmp);
}

template <class T>
constexpr decltype(auto) dist(const T &a, const T &b) noexcept {
  return (decltype(a.x))sqrt(P2(a.x, b.x) + P2(a.y, b.y) + P2(a.z, b.z));
}

template <class T, class D>
constexpr T move(const D len, const long double heading) noexcept {
  constexpr const auto rad{PI_Constant / 180.0};
  return T{len * cos(heading * rad), len * sin(heading * rad)};
}

} // namespace Math