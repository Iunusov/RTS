#include "Collisions.hpp"
#include "IMovableObject.hpp"
#include "IStaticObject.hpp"
#include <tuple>

#include "Math.hpp"

void IMovableObject::execute() noexcept {
  Collisions::getInstance()->update(*this);
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

bool IMovableObject::collide(const IMovableObject &obj) const noexcept {
  if (getId() == obj.getId()) {
    return false;
  }
  return getPosition().distance(obj.getPosition()) <
         getRadius() + obj.getRadius();
}
