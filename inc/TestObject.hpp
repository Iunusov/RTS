#pragma once

#include "IMovableObject.hpp"
#include "ObjectBase.hpp"
#include "VideoContext.hpp"

class TestObject final : public ObjectBase, public IMovableObject {
public:
  TestObject(const Coord &pos) { position = pos; };
  virtual void moveTo(const Coord &) noexcept override;
  virtual void draw(VideoContext &ctx) const noexcept override {
    ctx.SetPosition(position);
    ctx.draw();
  }
};

class TestBuilding : public ObjectBase {};
