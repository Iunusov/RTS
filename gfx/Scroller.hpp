#pragma once

#include <mutex>

#include "Config.hpp"
#include "Coord.hpp"

class Scroller final {
private:
  float scale{1.0f};
  mutable std::recursive_mutex mtx;
  Coord coord{MAX_COORD / 2.0, MAX_COORD / 2.0};

public:
  Scroller() noexcept = default;
  Coord GetCameraPos() const noexcept;
  void SetCameraPos(const Coord &pos) noexcept;
  float getScale() const noexcept;
  void execute() noexcept;
};
