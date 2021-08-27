#pragma once

#include "Cloneable.hpp"
#include "ICommand.hpp"

#include <functional>

#include "CppHacks.hpp"

class CommandBase final : public Cloneable<CommandBase, ICommand, ICommand> {
protected:
  Priority priority = Priority::LONG_RUNNING;

private:
  std::function<bool(size_t)> execute_command;

public:
  CommandBase(ICommand::Priority set_priority,
              std::function<bool(size_t)> func) noexcept
      : priority(set_priority), execute_command(func) {}

  bool execute(size_t objectId) NCNOF { return execute_command(objectId); }

  Priority getPriority() CNOF { return priority; }
};
