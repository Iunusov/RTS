#pragma once

class Map {};

class IMovableObject;
struct Coord;

class IVideoContext {
public:
  virtual void setup() noexcept = 0;
  virtual void draw(const Map &obj) noexcept = 0;
  virtual void draw(const IMovableObject &obj) noexcept = 0;
  virtual void present() noexcept = 0;
  virtual void clear() noexcept = 0;
  virtual void setCamera(const Coord &) noexcept = 0;
  virtual void delay() const noexcept = 0;
  virtual ~IVideoContext() noexcept = default;
};