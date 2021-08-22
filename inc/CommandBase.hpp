#pragma once

#include "ICommand.hpp"

class CommandBase : public ICommand {
protected:
  Priority priority = Priority::LONG_RUNNING;

public:
  virtual Priority getPriority() const noexcept override { return priority; }
};
