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

public:
  virtual double fireAngle() const noexcept final { return fire_angle; }

  virtual void moveForward() noexcept final { position.x += getDiff(); }

  virtual void moveBackward() noexcept final { position.x -= getDiff(); }
};