#pragma once

#include <cstddef>

class ICommand;
struct Coord;
class IVideoContext;

class IObject {
public:
  virtual size_t getId() const noexcept = 0;
  virtual void execute() noexcept = 0;
  virtual void acceptCommand(const ICommand &command) noexcept = 0;
  virtual ~IObject() noexcept = default;
  virtual Coord getPosition() const noexcept = 0;
  virtual int64_t getHealth() const noexcept = 0;
  virtual void draw(IVideoContext &) const noexcept = 0;
};
