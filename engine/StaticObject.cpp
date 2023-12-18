#include "StaticObject.hpp"

#include "IVideoContext.hpp"
#include "MovableObject.hpp"

bool StaticObject::collide(const MovableObject &obj) const noexcept {
  const auto &points{getPoints()};
  for (const auto &p : points) {
    if ((p + getPositionRef() - Coord{getWidth() / 2.0, getHeight() / 2.0})
            .distanceSquare(obj.getPositionRef()) <
        obj.getRadius() * obj.getRadius()) {
      return true;
    }
  }
  return false;
}

void StaticObject::draw(IVideoContext &ctx, double timeDiff) noexcept {
  ctx.draw(this, timeDiff);
}
