#pragma once

#include <cstddef>

class ICommand {

public:
  enum class Priority { ONE_CYCLE, IDLE, LONG_RUNNING };

public:
  virtual Priority getPriority() const noexcept = 0;
  virtual bool execute(size_t objectId) noexcept = 0;
  virtual ICommand *clone() const noexcept = 0;
  virtual ~ICommand() noexcept = default;
};
