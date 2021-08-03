#include "TestObject.hpp"

#include <iostream>

void TestObject::moveTo(const Coord &) noexcept {
  position.x += 100;
  position.y += 200;
  position.z += 300;
}
