#pragma once

#include "ObjectBase.hpp"

#include <cstddef>
#include <cstdint>

class MovableObject : public ObjectBase {

public:
  double fire_angle{0};

private:
  double prev_fire_angle{fire_angle};

public:
  bool collide(const MovableObject &obj) CNOF;
  bool isMovable() CNOF { return true; }
  void execute() NCNOF;
  void draw(IVideoContext &ctx, double timeDiff) NCNOF;
  virtual void moveForward() noexcept final;
  virtual void moveBackward() noexcept final;
  virtual void rotateLeft() noexcept final;
  virtual void rotateRiht() noexcept final;
  virtual void move(const bool forward = true) noexcept final;
  virtual double fireAngle() const noexcept final;
  virtual double getDiff() const noexcept final;
  virtual double getRadius() const noexcept { return 0; }
};
