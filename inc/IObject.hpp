#pragma once

class IResponse;
class ICommand;
struct Coord;
class VideoContext;

class IObject {
public:
  inline virtual size_t getId() const noexcept = 0;
  inline virtual void execute() noexcept = 0;
  virtual void acceptCommand(const ICommand &command) noexcept = 0;
  virtual ~IObject() noexcept = default;
  inline virtual Coord getPosition() const noexcept = 0;
  virtual void draw(VideoContext &) const noexcept = 0;
};