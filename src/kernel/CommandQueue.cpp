#include "CommandQueue.hpp"

#include "IObject.hpp"

CommandQueue::CommandQueue(const CommandQueue &src) noexcept {
  for (const auto &cmd : src.idleCmdQueue) {
    idleCmdQueue.emplace_back(cmd->clone());
  }
  for (const auto &cmd : src.cmdQueue) {
    cmdQueue.emplace_back(cmd->clone());
  }
}

void CommandQueue::execute(IObject &object) noexcept {
  for (auto &cmd : idleCmdQueue) {
    cmd->execute(object);
  }
  if (cmdQueue.empty()) {
    return;
  }
  std::unique_ptr<ICommand> &p = cmdQueue.front();
  if (p->execute(object)) {
    cmdQueue.pop_front();
  }
}
