#pragma once

namespace Math {
constexpr double PI_Constant{3.14159265358979323846};

constexpr inline long double lerp(const long double a, const long double b,
                                  const long double t) noexcept {
  return (a + t * (b - a));
}
} // namespace Math