#pragma once

#include "Collisions.hpp"
#include "Config.hpp"
#include "Coord.hpp"
#include "IMovableObject.hpp"
#include "IVideoContext.hpp"
#include "ObjectBase.hpp"

#include <vector>

class IStaticObject : public ObjectBase {
protected:
public:
  void execute() NCNOF {}

  virtual bool collide(const IMovableObject &obj) CNOF {
    const auto & points{getPoints()};
    for (const auto &p : points) {
      if ((p + getPosition() - Coord{getWidth() / 2.0, getHeight() / 2.0})
              .distance(obj.getPosition()) < obj.getRadius()) {
        return true;
      }
    }
    return false;
  }

  virtual const std::vector<Coord> &getPoints() const noexcept = 0;
  virtual double getWidth() const noexcept = 0;
  virtual double getHeight() const noexcept = 0;

  void draw(IVideoContext &ctx) NCNOF { ctx.draw(this); }

  bool isMovable() CNOF { return false; }
};
