#pragma once

#include "IObject.hpp"
#include "IVideoContext.hpp"

#include "Coord.hpp"

#include <iostream>
#include <vector>

class Renderer2D final {
private:
  IVideoContext *ctx = nullptr;
  Map map;

public:
  Renderer2D(IVideoContext *ctx) noexcept : ctx{ctx} {}

  void Delay(size_t ms) { ctx->delay(ms); }

  void Render(const Coord &pos,
              const std::vector<IObject *> &Objects) noexcept {
    ctx->clear();
    ctx->setCamera(pos);
    ctx->draw(map);
    size_t count{0};
    for (const auto &obj : Objects) {
      if (obj->getPosition().x > pos.x + 1920 ||
          obj->getPosition().y > pos.y + 1080 ||
          obj->getPosition().x < pos.x - 400 ||
          obj->getPosition().y < pos.y - 400) {
        continue;
      }
      obj->draw(*ctx);
      ++count;
    }
    std::cout << "objects in frame: " << count << std::endl;
    ctx->present();
  }
};
