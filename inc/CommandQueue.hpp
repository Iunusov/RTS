#pragma once

#include <list>
#include <memory>
#include <vector>

#include "ICommand.hpp"

class IObject;
class CommandQueue final {

private:
  std::list<std::unique_ptr<ICommand>> cmdQueue;
  std::vector<std::unique_ptr<ICommand>> idleCmdQueue;

public:
  CommandQueue() noexcept = default;
  CommandQueue(const CommandQueue &src) noexcept;

  void addIdleCommand(ICommand *cmd) noexcept {
    idleCmdQueue.emplace_back(cmd);
  }

  void addCommand(ICommand *cmd) noexcept { cmdQueue.emplace_back(cmd); }
  bool empty() const noexcept { return cmdQueue.empty(); };
  void execute(IObject &object) noexcept;
};