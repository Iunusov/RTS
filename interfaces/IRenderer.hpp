#pragma once

#include <chrono>
#include <vector>
class IObject;

class IRenderer {

public:
  virtual void Delay(size_t ms) noexcept = 0;
  virtual bool isVisible(const IObject &obj) const noexcept = 0;
  virtual void
  Render(const Coord &pos, float scale, const std::vector<IObject *> &Objects,
         const std::chrono::time_point<std::chrono::steady_clock> ts) = 0;
};
