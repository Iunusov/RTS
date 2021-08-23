#include "TestObject.hpp"
#include "VideoContext.hpp"

#include <random>

void TestObject::moveTo(const Coord &) noexcept {
  static std::mt19937 rng((unsigned int)time(NULL));
  static std::uniform_int_distribution<int64_t> gen(-50, 50);

  position.x += gen(rng);
  position.y += gen(rng);
  position.z += 0;

  if (position.x < 0)
    position.x = 0;
  if (position.y < 0)
    position.y = 0;
  if (position.x >= 1920 - 2)
    position.x = 1920 - 2;
  if (position.y >= 1080 - 2)
    position.y = 1080 - 2;
}

void TestObject::draw(VideoContext &ctx) const noexcept { ctx.draw(*this); }
