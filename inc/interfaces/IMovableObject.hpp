#pragma once

#include "Config.hpp"
#include "ObjectBase.hpp"

class IMovableObject : public ObjectBase {
private:
  constexpr size_t getDiff() const noexcept {
    static_assert(400 % MODEL_EXECUTE_PER_SECOND == 0,
                  "only integer diff allowed");
    return 400 / MODEL_EXECUTE_PER_SECOND;
  }

public:
  double fire_angle{};
  double prev_fire_angle{};

  // IMovableObject(const IMovableObject &src):
  // prev_fire_angle{src.prev_fire_angle}{}

public:
  void execute() NCNOF {
    prev_fire_angle = fire_angle;
    ObjectBase::execute();
  }

  virtual double fireAngle() const noexcept final { return fire_angle; }
  virtual double prevFireAngle() const noexcept final {
    return prev_fire_angle;
  }

  virtual void moveForward() noexcept final { position.x += getDiff(); }

  virtual void moveBackward() noexcept final { position.x -= getDiff(); }
};