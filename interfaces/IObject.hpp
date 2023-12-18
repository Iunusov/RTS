#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>

class ICommand;
struct Coord;
class IVideoContext;
class MovableObject;

class IObject {
public:
  virtual size_t getId() const noexcept = 0;
  virtual void execute() noexcept = 0;
  virtual void acceptCommand(const ICommand &command) noexcept = 0;
  virtual const Coord &getPositionRef() const noexcept = 0;
  virtual void setPosition(const Coord &) noexcept = 0;
  virtual int64_t getHealth() const noexcept = 0;
  virtual double getHeading() const noexcept = 0;
  virtual void setHeading(double) noexcept = 0;
  virtual void teleportTo(const Coord &pos) noexcept = 0;
  virtual void draw(IVideoContext &, double timeDiff) noexcept = 0;
  virtual IObject *clone() const noexcept = 0;
  virtual bool collide(const MovableObject &) const noexcept = 0;
  virtual bool isMovable() const noexcept = 0;
  virtual std::optional<size_t> getCollisionsBucketID() const noexcept = 0;
  virtual void setCollisionsBucketID(size_t b_id) noexcept = 0;
  virtual ~IObject() noexcept = default;
};
