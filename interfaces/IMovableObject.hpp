#pragma once

#include "Collisions.hpp"
#include "Config.hpp"
#include "ObjectBase.hpp"

class IMovableObject : public ObjectBase {
private:
  double prev_fire_angle{fire_angle};

  constexpr int64_t getDiff() noexcept {
    static_assert(400 % MODEL_EXECUTE_PER_SECOND == 0,
                  "only integer diff allowed");
    return 400 / MODEL_EXECUTE_PER_SECOND;
  }

  virtual void move(const bool forward = true) noexcept {
    const auto target{
        getPosition() +
        Math::move<Coord>((forward ? 1 : -1) * getDiff(), getHeading())};
    if (!Collisions::getInstance()->checkCollisions(target, getId())) {
      setPosition(target);
    }
  }

public:
  double fire_angle{0};

  void approx(double timeDiff) NCNOF;

  void execute() NCNOF;

  virtual double fireAngle() const noexcept final;
  virtual void moveForward() noexcept final;
  virtual void moveBackward() noexcept final;
  virtual void rotateLeft() noexcept final;
  virtual void rotateRiht() noexcept final;
};
