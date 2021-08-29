#pragma once

#include "Cloneable.hpp"
#include "ICommand.hpp"
#include "ObjectBase.hpp"

#include <functional>

#include "CppHacks.hpp"

template <class T>
class CommandBase final : public Cloneable<CommandBase<T>, ICommand, ICommand> {
protected:
  ICommand::Priority priority = ICommand::Priority::LONG_RUNNING;
  T t;

private:
public:
  CommandBase(ICommand::Priority set_priority) noexcept
      : priority(set_priority) {}

  bool execute(IObject &obj) NCNOF { return t.execute(obj); }

  ICommand::Priority getPriority() CNOF { return priority; }
};
