#pragma once

#include "Coord.hpp"
#include "IMovableObject.hpp"

class TestObject final : public IMovableObject {
public:
  TestObject(const Coord &pos) noexcept;
};
