#pragma once

#include "Cloneable.hpp"
#include "IObject.hpp"
#include "MovableObject.hpp"

class TestObject final : public Cloneable<TestObject, IObject, MovableObject> {
public:
  double getRadius() const noexcept override { return 125; }
};
