#pragma once

class IResponse;
class ICommand;
struct Coord;
class VideoContext;

class IObject {
public:
  virtual size_t getId() const noexcept = 0;
  virtual void execute() noexcept = 0;
  virtual void acceptCommand(const ICommand &command) noexcept = 0;
  virtual ~IObject() noexcept = default;
  virtual Coord getPosition() const noexcept = 0;
  virtual size_t fireAngle() const noexcept = 0;
  virtual void draw(VideoContext &) const noexcept = 0;
};