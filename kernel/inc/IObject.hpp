#pragma once

class IResponse;
class ICommand;

class IObject {
public:
  virtual void execute() noexcept = 0;
  virtual IResponse acceptCommand(const ICommand &command) noexcept = 0;
  virtual ~IObject() noexcept = default;
};