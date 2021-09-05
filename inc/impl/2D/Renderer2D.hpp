#pragma once

#include "Coord.hpp"
#include "IObject.hpp"
#include "IRenderer.hpp"
#include "IVideoContext.hpp"

//#include <iostream>
#include <vector>

class Renderer2D final : public IRenderer {
private:
  IVideoContext *ctx = nullptr;
  Map map;

public:
  Renderer2D(IVideoContext *ctx) noexcept : ctx{ctx} {}
  void Delay(size_t ms) noexcept override { ctx->delay(ms); }

  bool isVisible(const IObject &obj) const noexcept override {
    return obj.isVisible(*ctx);
  }

  void Render(const Coord &pos, const std::vector<IObject *> &Objects,
              double timeDiff) noexcept override {

    ctx->clear();
    ctx->setCamera(pos);
    ctx->draw(map);
    for (const auto &obj : Objects) {
      obj->approx(timeDiff);
      obj->draw(*ctx);
    }
    // std::cout << "objects in frame: " << Objects.size() << std::endl;
    ctx->present();
  }
};
