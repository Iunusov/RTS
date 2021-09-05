#include "IMovableObject.hpp"

#include <cmath>

#include "Math.hpp"

namespace {
constexpr size_t getDiff() noexcept {
  static_assert(400 % MODEL_EXECUTE_PER_SECOND == 0,
                "only integer diff allowed");
  return 400 / MODEL_EXECUTE_PER_SECOND;
}
} // namespace

void IMovableObject::approx(double timeDiff) noexcept {
  fire_angle = (double)Math::lerp(prev_fire_angle, fire_angle, timeDiff);
  ObjectBase::approx(timeDiff);
}

void IMovableObject::execute() noexcept {
  prev_fire_angle = fire_angle;
  ObjectBase::execute();
}

double IMovableObject::fireAngle() const noexcept { return fire_angle; }

void IMovableObject::moveForward() noexcept {
  Coord result = getPosition();
  result.x += (getDiff() * cos(getHeading() * (Math::PI_Constant / 180.0)));
  result.y += (getDiff() * sin(getHeading() * (Math::PI_Constant / 180.0)));
  setPosition(result);
}

void IMovableObject::moveBackward() noexcept {
  Coord result = getPosition();
  result.x -= (getDiff() * cos(getHeading() * (Math::PI_Constant / 180.0)));
  result.y -= (getDiff() * sin(getHeading() * (Math::PI_Constant / 180.0)));
  setPosition(result);
}
void IMovableObject::rotateLeft() noexcept {
  setHeading(getHeading() -
             (double)360.0 / 10 / (double)MODEL_EXECUTE_PER_SECOND);
};
void IMovableObject::rotateRiht() noexcept {
  setHeading(getHeading() +
             (double)360.0 / 10 / (double)MODEL_EXECUTE_PER_SECOND);
};
