#pragma once

#include <SDL.h>
#include <mutex>

#include "Coord.hpp"
#include "IObject.hpp"
#include "IVideoContext.hpp"
class IMovableObject;
class IStaticObject;

class VideoContextSDL final : public IVideoContext {
private:
  static IVideoContext *instance;
  SDL_Renderer *rend = nullptr;
  float m_scale{1};
  Coord cameraPosition;
  SDL_Window *win = nullptr;
  int w{};
  int h{};
  int m_fps = 100;

public:
  static void Create() noexcept;
  static IVideoContext *GetInstance() noexcept { return instance; }

  ~VideoContextSDL() noexcept override;

  void setCamera(const Coord &pos) noexcept override { cameraPosition = pos; }

  void setScale(float scale) noexcept override {
    SDL_RenderSetScale(rend, scale, scale);
    m_scale = scale;
  }

  int getWidth() const noexcept override { return w; }
  int getHeigt() const noexcept override { return h; }
  int getFps() const noexcept override { return m_fps; }

  void draw(const Map &obj) noexcept override;
  bool isVisible(const Coord &obj) const noexcept override {

    const auto posx = obj.x;
    const auto posy = obj.y;

    return (posx >= cameraPosition.x - w / 2.0 / (double)m_scale - 1000) &&
           (posx <= cameraPosition.x + w / 2.0 / (double)m_scale + 1000) &&
           (posy >= cameraPosition.y - h / 2.0 / (double)m_scale - 1000) &&
           (posy <= cameraPosition.y + h / 2.0 / (double)m_scale + 1000);
  }
  inline void clear() noexcept override;
  void delay(size_t ms) const noexcept override;
  void present() noexcept override;
  void draw(const IMovableObject *obj) noexcept override;
  void draw(const IStaticObject *obj) noexcept override;
  void setup() noexcept override;
};
