#pragma once

#include "Config.hpp"
#include "ObjectBase.hpp"

class IMovableObject : public ObjectBase {
private:
  double prev_fire_angle{fire_angle};

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
