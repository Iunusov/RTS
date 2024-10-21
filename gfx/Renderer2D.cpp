#include "Renderer2D.hpp"
#include "Config.hpp"
#include "Math.hpp"
#include <cmath>

void Renderer2D::Render(
    const Coord &pos, float scale, const std::vector<IObject *> &Objects,
    const std::chrono::time_point<std::chrono::steady_clock> &ts) noexcept {
  static Coord prev_camera_position{pos};
  ctx->clear();

  ctx->setScale(scale * scale);

  const auto diff{(double)std::chrono::duration_cast<std::chrono::milliseconds>(
                      std::chrono::steady_clock::now() - ts)
                      .count() /
                  (double)MODEL_CYCLE_TIME_MS};
  for (const auto &obj : Objects) {
    obj->draw(*ctx, diff);
  }

  const auto approx_cam_position =
      Coord{std::lerp(prev_camera_position.x, pos.x, diff),
            std::lerp(prev_camera_position.y, pos.y, diff)};

  prev_camera_position = pos;
  ctx->setCamera(approx_cam_position);

  ctx->present();
}
