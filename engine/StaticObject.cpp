#include "StaticObject.hpp"

#include "IVideoContext.hpp"
#include "Math.hpp"
#include "MovableObject.hpp"

bool StaticObject::collide(const MovableObject &obj) const noexcept {
  const auto &points{getPoints()};
  for (const auto &p : points) {
    Coord tmp{getPositionRef()};
    tmp += p;
    tmp -= Coord{getWidth() / 2.0, getHeight() / 2.0};
    if (Math::dist(obj.getPositionRef(), tmp) <
        obj.getRadius() * obj.getRadius()) {
      return true;
    }
  }
  return false;
}

void StaticObject::draw(IVideoContext &ctx, double timeDiff) noexcept {
  ctx.draw(this, timeDiff);
}
