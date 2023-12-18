#include "ObjectBase.hpp"

void ObjectBase::acceptCommand(const ICommand &command) noexcept {
  if (command.getPriority() == ICommand::Priority::ONE_CYCLE) {
    command.clone()->execute(*this);
    return;
  }
  if (command.getPriority() == ICommand::Priority::IDLE) {
    cmds.addIdleCommand(command.clone());
    return;
  }

  cmds.addCommand(command.clone());
}

void ObjectBase::execute() noexcept {
  previousPosition = position;
  previousHeading = heading;
  cmds.execute(*this);
}