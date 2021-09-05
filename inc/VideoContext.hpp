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
  int getHeigt() const noexcept override { return w; }
  Coord getCameraPos() const noexcept {
    std::lock_guard<std::mutex> lock{mtx};
    return cameraPosition;
  }
  void draw(const Map &obj) noexcept override;
  bool isVisible(const IObject &obj) const noexcept override {
    std::lock_guard<std::mutex> lock{mtx};
    constexpr size_t border = 500;
    if (obj.getPosition().x > cameraPosition.x + w + border ||
        obj.getPosition().y > cameraPosition.y + h + border ||
        obj.getPosition().x < cameraPosition.x - border ||
        obj.getPosition().y < cameraPosition.y - border) {
      return false;
    }
    return true;
  }

  void clear() noexcept override;
  void delay(size_t ms) const noexcept override;
  void present() noexcept override;
  void draw(const IObject *obj) noexcept override;
  void setup() noexcept override;
};
