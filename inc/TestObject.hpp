#pragma once

#include "IMovableObject.hpp"
#include "ObjectBase.hpp"

#include "CppHacks.hpp"

class VideoContext;

extern const ICommand *command_idle;

class TestObject final : public IMovableObject {
public:
  TestObject(const Coord &pos) noexcept;

  void moveTo(const Coord &) NCNOF;
};
