#pragma once

#include "Collisions.hpp"
#include "Config.hpp"
#include "IVideoContext.hpp"
#include "ObjectBase.hpp"
class IStaticObject;

class IMovableObject : public ObjectBase {
private:
  constexpr int64_t getDiff() noexcept {
    static_assert(400 % MODEL_EXECUTE_PER_SECOND == 0,
                  "only integer diff allowed");
    return 400 / MODEL_EXECUTE_PER_SECOND;
  }

  virtual void move(const bool forward = true) noexcept {
    const auto currentPosition{getPosition()};
    setPosition(
        currentPosition +
        Math::move<Coord>((forward ? 1 : -1) * getDiff(), getHeading()));
    if (Collisions::getInstance()->checkCollisions(*this)) {
      setPosition(currentPosition);
    }
  }

public:
  double fire_angle{0};

private:
  double prev_fire_angle{fire_angle};

public:
  void execute() NCNOF;
  virtual bool collide(const IMovableObject &obj) CNOF;

  virtual double getRadius() const noexcept = 0;
  virtual double fireAngle() const noexcept final;
  virtual void moveForward() noexcept final;
  virtual void moveBackward() noexcept final;
  virtual void rotateLeft() noexcept final;
  virtual void rotateRiht() noexcept final;

  void draw(IVideoContext &ctx, double timeDiff) NCNOF {
    ctx.draw(this, timeDiff);
  }
  bool isMovable() CNOF { return true; }
};
