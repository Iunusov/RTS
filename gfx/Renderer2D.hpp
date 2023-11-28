#pragma once

#include "Coord.hpp"
#include "IObject.hpp"
#include "IRenderer.hpp"
#include "IVideoContext.hpp"

//#include <iostream>
#include <vector>
#include <chrono>

class Renderer2D final : public IRenderer {
private:
  IVideoContext *ctx = nullptr;

public:
  Renderer2D(IVideoContext *ctx_) noexcept : ctx{ctx_} {}
  void Delay(size_t ms) noexcept override { ctx->delay(ms); }

  bool isVisible(const IObject &obj) const noexcept override {
    return ctx->isVisible(obj.getPosition());
  }

  void Render(const Coord &pos, float, const std::vector<IObject *> &Objects,
              const std::chrono::time_point<std::chrono::steady_clock> &ts) noexcept override;
};
