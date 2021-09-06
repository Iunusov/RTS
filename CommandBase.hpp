#pragma once

#include <functional>

#include "Cloneable.hpp"
#include "CppHacks.hpp"
#include "ICommand.hpp"
#include "ObjectBase.hpp"

template <class T>
class CommandBase final : public Cloneable<CommandBase<T>, ICommand, ICommand> {
protected:
  ICommand::Priority priority = ICommand::Priority::LONG_RUNNING;
  T t;

public:
  CommandBase(ICommand::Priority set_priority) noexcept
      : priority(set_priority) {}

  bool execute(IObject &obj) NCNOF { return t.execute(obj); }

  ICommand::Priority getPriority() CNOF { return priority; }
};
