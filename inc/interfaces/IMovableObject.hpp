#pragma once

#include "Config.hpp"
#include "ObjectBase.hpp"
#include <cmath>
#include <iostream>
#include <random>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class IMovableObject : public ObjectBase {
private:
  constexpr size_t getDiff() const noexcept {
    static_assert(400 % MODEL_EXECUTE_PER_SECOND == 0,
                  "only integer diff allowed");
    return 400 / MODEL_EXECUTE_PER_SECOND;
  }

  static std::mt19937 rng2;
  static std::uniform_int_distribution<int64_t> gen2;

  // IMovableObject(const IMovableObject &src):
  // prev_fire_angle{src.prev_fire_angle}{}

public:
  double fire_angle{0};
  double heading{(double)gen2(rng2)};

  double prev_fire_angle{fire_angle};
  double prev_heading{heading};

  void execute() NCNOF {
    prev_fire_angle = fire_angle;
    prev_heading = heading;
    ObjectBase::execute();
  }

  virtual double fireAngle() const noexcept final { return fire_angle; }
  virtual double prevFireAngle() const noexcept final {
    return prev_fire_angle;
  }

  virtual void moveForward() noexcept final {
    position.x += (getDiff() * cos(heading * (M_PI / 180.0)));
    position.y += (getDiff() * sin(heading * (M_PI / 180.0)));

    // std::cout<<"forward"<<std::endl;
  }

  virtual void moveBackward() noexcept final {
    position.x -= (getDiff() * cos(heading * (M_PI / 180.0)));
    position.y -= (getDiff() * sin(heading * (M_PI / 180.0)));
    // std::cout<<"backward"<<std::endl;
  }
  virtual void rotateLeft() noexcept final {
    heading -= (double)360.0 / 10 / (double)MODEL_EXECUTE_PER_SECOND;
  };
  virtual void rotateRiht() noexcept final {
    heading += (double)360.0 / 10 / (double)MODEL_EXECUTE_PER_SECOND;
  };
  virtual double getHeading() const noexcept final { return heading; };
  virtual void setHeading(double angle) noexcept final { heading = angle; };
};
