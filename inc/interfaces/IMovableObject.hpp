#pragma once
#include "CppHacks.hpp"
#include "IVideoContext.hpp"
#include "ObjectBase.hpp"

#include "Config.hpp"
#include "Coord.hpp"

#include <cmath>
#include <cstddef>

struct Coord;

class IMovableObject : public ObjectBase {
private:
  long double pos{};

public:
  double fire_angle{};

public:
  virtual double fireAngle() const noexcept final { return fire_angle; }
  virtual void draw(IVideoContext &ctx) CNOF { ctx.draw(*this); }

  virtual void moveForward() noexcept final {
    position.x -= (int64_t)round(pos);
    pos += 0.4 * (long double)400.0 / (long double)MODEL_EXECUTE_PER_SECOND;
    position.x += (int64_t)round(pos);
  }

  virtual void moveBackward() noexcept final {
    position.x += (int64_t)round(pos);
    pos += 0.4 * (long double)400.0 / (long double)MODEL_EXECUTE_PER_SECOND;
    position.x -= (int64_t)round(pos);
  }
};