#include "ObjectBase.hpp"

void ObjectBase::acceptCommand(const ICommand &command) noexcept {
  if (command.getPriority() == ICommand::Priority::ONE_CYCLE) {
    command.clone()->execute(*this);
    return;
  }
  if (command.getPriority() == ICommand::Priority::IDLE) {
    idleCmdQueue.emplace_back(command.clone());
    return;
  }

  cmdQueue.emplace(command.clone());
}

void ObjectBase::execute() noexcept {
  previousPosition = getPosition();
  for (auto &cmd : idleCmdQueue) {
    cmd->execute(*this);
  }
  if (cmdQueue.empty()) {
    return;
  }
  std::unique_ptr<ICommand> &p = cmdQueue.front();
  if (p->execute(*this)) {
    cmdQueue.pop();
  }
}
