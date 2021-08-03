#include "ObjectBase.hpp"

#include "IResponse.hpp"

IResponse ObjectBase::acceptCommand(const ICommand &command) noexcept {
  if (command.getPriority() == ICommand::Priority::ONE_CYCLE) {
    return command.clone()->execute(this);
  }
  cmdQueue.push(command.clone());
  return IResponse{"accepted"};
}
void ObjectBase::execute() noexcept {
  if (cmdQueue.empty()) {
    return;
  }
  std::unique_ptr<ICommand> &p = cmdQueue.front();
  p->execute(this);
  // cmdQueue.pop();
}