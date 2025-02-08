#pragma once

#include "Config.hpp"
#include "Coord.hpp"

class Scroller final {
private:
  float m_scale{1.0f};
  Coord m_position{MAX_COORD / 2.0f, MAX_COORD / 2.0f};
  float m_v_direction{0}, m_h_direction{0}, m_zoom_direction{0};
  float m_time_per_frame_ms{0};

public:
  Scroller(float ms) noexcept : m_time_per_frame_ms{ms} {}
  Coord GetCameraPos() const noexcept;
  void SetCameraPos(const Coord &pos) noexcept;
  float getScale() const noexcept;
  void execute() noexcept;

  bool escape_key_pressed{};
};
