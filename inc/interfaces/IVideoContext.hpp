#pragma once

class Map {};

class IObject;
struct Coord;

class IVideoContext {
public:
  virtual void setup() noexcept = 0;
  virtual void draw(const Map &obj) noexcept = 0;
  virtual void draw(const IObject *obj) noexcept = 0;
  virtual void present() noexcept = 0;
  virtual void clear() noexcept = 0;
  virtual void setCamera(const Coord &) noexcept = 0;
  virtual void delay(size_t) const noexcept = 0;
  virtual ~IVideoContext() noexcept = default;
};