#pragma once

#include "Coord.hpp"
#include "ICommand.hpp"
#include "IObject.hpp"

class IResponse;

#include <queue>

namespace {
size_t getGlobalId() noexcept {
  static size_t id{0};
  return ++id;
}
} // namespace

class ObjectBase : public IObject {
private:
  const size_t id;
  std::queue<CMD> cmdQueue;

public:
  ObjectBase() noexcept : id{getGlobalId()} {}
  virtual size_t getId() const noexcept override { return id; }
  virtual void execute() noexcept override;
  virtual IResponse acceptCommand(const ICommand &command) noexcept override;

  int64_t health = 100;
  Coord position{};
  virtual int64_t getHealth() const noexcept { return health; }
  virtual Coord getPosition() const noexcept override { return position; }
};