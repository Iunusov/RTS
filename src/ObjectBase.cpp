#include "ObjectBase.hpp"

#include "IResponse.hpp"

void ObjectBase::acceptCommand(const ICommand &command) noexcept {
  if (command.getPriority() == ICommand::Priority::ONE_CYCLE) {
    command.clone()->execute(this);
    return;
  }
  cmdQueue.push(command.clone());
}
void ObjectBase::execute() noexcept {
  if (cmdQueue.empty()) {
    return;
  }
  std::unique_ptr<ICommand> &p = cmdQueue.front();
  if (p->execute(this)) {
    cmdQueue.pop();
  }
}
