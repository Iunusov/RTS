#pragma once

#include <cstddef>
#include <cstdint>

class ICommand;
struct Coord;
class IVideoContext;
class IMovableObject;
class IStaticObject;

class IObject {
public:
  virtual size_t getId() const noexcept = 0;
  virtual void execute() noexcept = 0;
  virtual void acceptCommand(const ICommand &command) noexcept = 0;
  virtual ~IObject() noexcept = default;
  virtual Coord getPosition() const noexcept = 0;
  virtual void setPosition(const Coord &) noexcept = 0;
  virtual int64_t getHealth() const noexcept = 0;
  virtual double getHeading() const noexcept = 0;
  virtual void setHeading(double) noexcept = 0;
  virtual void teleportTo(const Coord &pos) noexcept = 0;
  virtual void approx(double timeDIff) noexcept = 0;
  virtual void draw(IVideoContext &) noexcept = 0;
  virtual IObject *clone() const noexcept = 0;
  virtual bool collide(const IMovableObject &) const noexcept = 0;
  virtual bool isMovable() const noexcept = 0;
};
