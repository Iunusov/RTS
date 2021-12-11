#include "Renderer2D.hpp"

void Renderer2D::Render(const Coord &pos, float scale,
                        const std::vector<IObject *> &Objects,
                        const double timeDiff) noexcept {

  ctx->clear();
  ctx->setCamera(pos);
  ctx->setScale(scale);
  ctx->draw(map);
  for (const auto &obj : Objects) {
    obj->approx(timeDiff);
    obj->draw(*ctx);
  }
  ctx->present();
}
