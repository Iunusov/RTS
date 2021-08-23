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
  inline virtual size_t getId() const noexcept override final { return id; }

  inline virtual void execute() noexcept override {
    if (cmdQueue.empty()) {
      return;
    }
    std::unique_ptr<ICommand> &p = cmdQueue.front();
    if (p->execute(this)) {
      cmdQueue.pop();
    }
  }
  virtual void acceptCommand(const ICommand &command) noexcept override final;

  int64_t health = 100;
  Coord position{};
  virtual int64_t getHealth() const noexcept { return health; }
  inline virtual Coord getPosition() const noexcept override final {
    return position;
  }
};