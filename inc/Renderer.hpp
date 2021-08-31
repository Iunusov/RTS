#pragma once

#include "IObject.hpp"
#include "IVideoContext.hpp"

#include "Coord.hpp"

#include <list>

class Renderer2D final {
private:
  IVideoContext *ctx = nullptr;
  Map map;

public:
  Renderer2D(IVideoContext *ctx) noexcept : ctx{ctx} {}

  void Delay(size_t ms) { ctx->delay(ms); }

  void Render(const Coord &pos, const std::list<IObject *> &Objects) noexcept {
    ctx->clear();
    ctx->setCamera(pos);
    ctx->draw(map);
    for (auto &obj : Objects) {
      obj->draw(*ctx);
    }
    ctx->present();
  }
};
