#include "TestObject.hpp"

void TestObject::moveTo(const Coord &) noexcept {
  position.x += 1;
  position.y += 1;
  position.z += 0;
}
