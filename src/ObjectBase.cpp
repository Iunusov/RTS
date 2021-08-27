#include "ObjectBase.hpp"

void ObjectBase::acceptCommand(const ICommand &command) noexcept {
  if (command.getPriority() == ICommand::Priority::ONE_CYCLE) {
    command.clone()->execute(id);
    return;
  }
  if (command.getPriority() == ICommand::Priority::IDLE) {
    idleCmdQueue.emplace_back(command.clone());
    return;
  }

  cmdQueue.emplace(command.clone());
}

void ObjectBase::execute() noexcept {
  for (auto &cmd : idleCmdQueue) {
    cmd->execute(id);
  }
  if (cmdQueue.empty()) {
    return;
  }
  std::unique_ptr<ICommand> &p = cmdQueue.front();
  if (p->execute(id)) {
    cmdQueue.pop();
  }
}

IObject *ObjectBase::getById(size_t id) noexcept {
  auto res = cache.find(id);
  if (res == std::end(cache)) {
    return nullptr;
  }
  return res->second;
}

std::unordered_map<size_t, IObject *> ObjectBase::cache;