#include "MovableObject.hpp"
#include "Collisions.hpp"
#include "Config.hpp"
#include "IVideoContext.hpp"
#include <cstddef>
#include <cstdint>
#include <tuple>

#include "Math.hpp"

void MovableObject::execute() noexcept {
  prev_fire_angle = fire_angle;
  ObjectBase::execute();
  Collisions::getInstance()->update(*this);
}

double MovableObject::fireAngle() const noexcept { return fire_angle; }

void MovableObject::moveForward() noexcept { move(); }

void MovableObject::moveBackward() noexcept { move(false); }
void MovableObject::rotateLeft() noexcept {
  setHeading(getHeading() -
             (double)360.0 / 10 / (double)MODEL_EXECUTE_PER_SECOND);
}
void MovableObject::rotateRiht() noexcept {
  setHeading(getHeading() +
             (double)360.0 / 10 / (double)MODEL_EXECUTE_PER_SECOND);
}

bool MovableObject::collide(const MovableObject &obj) const noexcept {
  if (getId() == obj.getId()) {
    return false;
  }
  const auto radius = getRadius() + obj.getRadius();
  return Math::dist(getPositionRef(), obj.getPositionRef()) < radius * radius;
}

double MovableObject::getDiff() const noexcept {
  static_assert(400 % MODEL_EXECUTE_PER_SECOND == 0,
                "only integer diff allowed");
  return 400 / MODEL_EXECUTE_PER_SECOND;
}

void MovableObject::move(const bool forward) noexcept {
  const auto currentPosition{getPositionRef()};
  Coord newPosition{currentPosition};
  newPosition +=
      Math::move<Coord>((forward ? 1 : -1) * getDiff(), getHeading());
  setPosition(newPosition);
  if (Collisions::getInstance()->checkCollisions(*this)) {
    setPosition(currentPosition);
  }
}

void MovableObject::draw(IVideoContext &ctx, double timeDiff) noexcept {
  ctx.draw(this, timeDiff);
}
