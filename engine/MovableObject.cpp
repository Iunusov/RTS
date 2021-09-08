#include "IMovableObject.hpp"

#include "Math.hpp"

void IMovableObject::approx(double timeDiff) noexcept {
  fire_angle = (double)Math::lerp(prev_fire_angle, fire_angle, timeDiff);
  ObjectBase::approx(timeDiff);
}

void IMovableObject::execute() noexcept {
  prev_fire_angle = fire_angle;
  ObjectBase::execute();
}

double IMovableObject::fireAngle() const noexcept { return fire_angle; }

void IMovableObject::moveForward() noexcept { move(); }

void IMovableObject::moveBackward() noexcept { move(false); }
void IMovableObject::rotateLeft() noexcept {
  setHeading(getHeading() -
             (double)360.0 / 10 / (double)MODEL_EXECUTE_PER_SECOND);
}
void IMovableObject::rotateRiht() noexcept {
  setHeading(getHeading() +
             (double)360.0 / 10 / (double)MODEL_EXECUTE_PER_SECOND);
}
