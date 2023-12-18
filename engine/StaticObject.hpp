#pragma once

#include "Coord.hpp"
#include "ObjectBase.hpp"
#include <vector>

class IVideoContext;
class MovableObject;

class StaticObject : public ObjectBase {
private:
  std::vector<Coord> points_;

public:
  bool collide(const MovableObject &obj) CNOF;
  bool isMovable() CNOF { return false; }
  void draw(IVideoContext &ctx, double timeDiff) NCNOF;
  void execute() NCNOF { ObjectBase::execute(); }

  virtual const std::vector<Coord> &getPoints() const noexcept {
    return points_;
  }
  virtual double getWidth() const noexcept { return 0; }
  virtual double getHeight() const noexcept { return 0; }
};
