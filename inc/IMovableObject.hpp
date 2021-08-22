#pragma once

struct Coord;

class IMovableObject {
public:
  virtual void moveTo(const Coord &) noexcept = 0;
};