#pragma once
#include "CppHacks.hpp"
#include "IVideoContext.hpp"
#include "ObjectBase.hpp"

#include <cstddef>

struct Coord;

class IMovableObject : public ObjectBase {
public:
  size_t fire_angle{};

public:
  virtual void moveTo(const Coord &) noexcept = 0;
  virtual size_t fireAngle() const noexcept { return fire_angle; }
  virtual void draw(IVideoContext &ctx) CNOF { ctx.draw(*this); }
};