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
              const double timeDiff) noexcept override;
};
