#pragma once

class IResponse;
class ICommand;

class IObject {
public:
  virtual size_t getId() const noexcept = 0;
  virtual void execute() noexcept = 0;
  virtual IResponse acceptCommand(const ICommand &command) noexcept = 0;
  virtual ~IObject() noexcept = default;
};