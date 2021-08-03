#pragma once

#include "ICommand.hpp"
#include "IObject.hpp"

class IResponse;

#include <queue>

namespace {
int64_t getGlobalId() noexcept {
  static int64_t id{-1};
  return ++id;
}
} // namespace

struct Coord final {
  int64_t x{0};
  int64_t y{0};
  int64_t z{0};

  Coord(int64_t x, int64_t y, int64_t z) noexcept : x(x), y(y), z(z) {}
  Coord() noexcept = default;
};

class ObjectBase : public IObject {
private:
  int64_t id{-1};
  std::queue<CMD> cmdQueue;

public:
  ObjectBase() noexcept : id{getGlobalId()} {}
  int64_t getId() const noexcept { return id; }
  virtual void execute() noexcept override;
  virtual IResponse acceptCommand(const ICommand &command) noexcept override;

  int64_t health = 100;
  Coord position{};
  virtual int64_t getHealth() const noexcept { return health; }
};