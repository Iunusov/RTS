#include "TestObject.hpp"

#include <ctime>
#include <random>

#include "VideoContext.hpp"

void TestObject::moveTo(const Coord &) noexcept {
  static std::mt19937 rng((unsigned int)time(NULL));
  static std::uniform_int_distribution<int64_t> gen(-10, 10);

  position.x += gen(rng);
  position.y += gen(rng);
}

void TestObject::draw(const Coord &camera, VideoContext &ctx) const noexcept {
  ctx.draw(camera, *this);
}

TestObject::TestObject(const Coord &pos) noexcept {
  position = pos;
  acceptCommand(*command_idle);
}
