#pragma once

#include "Coord.hpp"
#include "ICommand.hpp"
#include "IObject.hpp"

class IResponse;

#include <queue>
#include <vector>

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
  std::vector<CMD> idleCmdQueue;

public:
  ObjectBase() noexcept : id{getGlobalId()} {}
  virtual size_t getId() const noexcept override final { return id; }

  virtual void execute() noexcept override {
    for (auto &cmd : idleCmdQueue) {
      cmd->execute(this);
    }
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
  size_t fire_angle{0};
  virtual int64_t getHealth() const noexcept { return health; }
  virtual Coord getPosition() const noexcept override final { return position; }
  virtual size_t fireAngle() const noexcept override { return fire_angle; }
};