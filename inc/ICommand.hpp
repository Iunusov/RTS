#pragma once

#include <memory>

class IObject;
class IResponse;

class ICommand;
using CMD = std::unique_ptr<ICommand>;

class ICommand {
public:
  enum class Priority { ONE_CYCLE, LONG_RUNNING };
  virtual Priority getPriority() const noexcept = 0;
  virtual IResponse execute(IObject *) noexcept = 0;
  virtual CMD clone() const noexcept = 0;
  virtual ~ICommand() noexcept = default;
};
