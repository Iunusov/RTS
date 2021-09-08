#pragma once

#include <SDL.h>
#include <mutex>

#include "Coord.hpp"
#include "IObject.hpp"
#include "IVideoContext.hpp"

class VideoContextSDL final : public IVideoContext {
private:
  static IVideoContext *instance;
  SDL_Renderer *rend = nullptr;
  Coord cameraPosition;
  SDL_Window *win = nullptr;
  int w{};
  int h{};
  mutable std::mutex mtx;

public:
  static void Create() noexcept;
  static IVideoContext *GetInstance() noexcept { return instance; }

  ~VideoContextSDL() noexcept override;

  void setCamera(const Coord &pos) noexcept override {
    std::lock_guard<std::mutex> lock{mtx};
    cameraPosition = pos;
  }

  int getWidth() const noexcept override { return w; }
  int getHeigt() const noexcept override { return h; }
  Coord getCameraPos() const noexcept {
    std::lock_guard<std::mutex> lock{mtx};
    return cameraPosition;
  }
  void draw(const Map &obj) noexcept override;
  bool isVisible(const IObject &obj) const noexcept override {
    std::lock_guard<std::mutex> lock{mtx};
    constexpr size_t offscreen = 1000;
    const auto pos{obj.getPosition()};
    return (pos.x + offscreen > cameraPosition.x) &&
           (pos.y + offscreen > cameraPosition.y) &&
           (pos.x < cameraPosition.x + w + offscreen) &&
           (pos.y < cameraPosition.y + h + offscreen);
  }
  inline void clear() noexcept override;
  void delay(size_t ms) const noexcept override;
  void present() noexcept override;
  void draw(const IObject *obj) noexcept override;
  void setup() noexcept override;
};
