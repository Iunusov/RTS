#pragma once

#include "Cloneable.hpp"
#include "Coord.hpp"
#include "CppHacks.hpp"
#include "ICommand.hpp"
#include "IObject.hpp"
#include "IVideoContext.hpp"

#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

namespace {
size_t getGlobalId() noexcept {
  static size_t id{0};
  return ++id;
}
} // namespace

class ObjectBase : public IObject {
private:
  const size_t id{getGlobalId()};
  std::queue<std::unique_ptr<ICommand>> cmdQueue;
  std::vector<std::unique_ptr<ICommand>> idleCmdQueue;

public:
  int64_t health = 100;
  Coord position{};

public:
  ObjectBase(const ObjectBase &src) noexcept
      : id{src.id}, health{src.health}, position{src.position} {}

  ObjectBase() = default;

  size_t getId() CNOF { return id; }
  void execute() NCNOF;
  void acceptCommand(const ICommand &command) NCNOF;
  virtual int64_t getHealth() CNOF { return health; }
  Coord getPosition() CNOF { return position; }
  virtual void draw(IVideoContext &ctx) CNOF { ctx.draw(this); }
};
