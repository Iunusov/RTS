#pragma once

#include "IMovableObject.hpp"
#include "IObject.hpp"

class TestObject final : public Cloneable<TestObject, IObject, IMovableObject> {
public:
  double getRadius() const noexcept override { return 125; }
};
