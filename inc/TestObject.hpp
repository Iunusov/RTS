#pragma once

#include "Coord.hpp"
#include "IMovableObject.hpp"

class TestObject final : public Cloneable<TestObject, IObject, IMovableObject> {
public:
  TestObject(const Coord &pos) noexcept;
};
