#pragma once


class IObject;
class IMovableObject;
class IStaticObject;
struct Coord;

class IVideoContext {
public:
  virtual void setup() noexcept = 0;
  virtual void draw(const IMovableObject *obj, double timeDiff) noexcept = 0;
  virtual void draw(const IStaticObject *obj, double timeDiff) noexcept = 0;
  virtual bool isVisible(const Coord &) const noexcept = 0;
  virtual void present() noexcept = 0;
  virtual void clear() noexcept = 0;
  virtual void setCamera(const Coord &) noexcept = 0;
  virtual void setScale(float) noexcept = 0;
  virtual int getWidth() const noexcept = 0;
  virtual int getHeigt() const noexcept = 0;
  virtual int getFps() const noexcept = 0;
  virtual void delay(size_t) const noexcept = 0;
  virtual ~IVideoContext() noexcept = default;
};
