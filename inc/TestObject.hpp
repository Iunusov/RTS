#pragma once

#include "IMovableObject.hpp"
#include "ObjectBase.hpp"

class TestObject final : public ObjectBase, public IMovableObject {
public:
  virtual void moveTo(const Coord &) noexcept override;
};

class TestBuilding : public ObjectBase {};