#pragma once

#include "IMovableObject.hpp"
#include "ObjectBase.hpp"

class TestObject final : public ObjectBase, public IMovableObject {
public:
  TestObject(const Coord &pos) { position = pos; };
  virtual void moveTo(const Coord &) noexcept override;
};

class TestBuilding : public ObjectBase {};