#include "ObjectBase.hpp"

#include "IResponse.hpp"

void ObjectBase::acceptCommand(const ICommand &command) noexcept {
  if (command.getPriority() == ICommand::Priority::ONE_CYCLE) {
    command.clone()->execute(this);
    return;
  }
  if (command.getPriority() == ICommand::Priority::IDLE) {
    idleCmdQueue.push_back(command.clone());
    return;
  }

  cmdQueue.push(command.clone());
}
