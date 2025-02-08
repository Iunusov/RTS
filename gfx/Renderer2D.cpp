#include "Renderer2D.hpp"
#include "Config.hpp"
#include <cmath>

void Renderer2D::Render(
    const Coord &pos, float scale, const std::vector<IObject *> &Objects,
    const std::chrono::time_point<std::chrono::steady_clock> &ts) noexcept {
  ctx->clear();

  ctx->setScale(scale);
  ctx->setCamera(pos);

  const auto diff{(double)std::chrono::duration_cast<std::chrono::milliseconds>(
                      std::chrono::steady_clock::now() - ts)
                      .count() /
                  (double)MODEL_CYCLE_TIME_MS};
  for (const auto &obj : Objects) {
    obj->draw(*ctx, diff);
  }

  ctx->present();
}
