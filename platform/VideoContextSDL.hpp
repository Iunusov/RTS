#pragma once

#include <SDL.h>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/x.H>

#include "Coord.hpp"
#include "IVideoContext.hpp"
class MovableObject;
class StaticObject;

class VideoContextSDL final : public IVideoContext {
private:
  static IVideoContext *instance;
  SDL_Renderer *rend = nullptr;
  float m_scale{1};
  Coord cameraPosition;
  SDL_Window *win = nullptr;
  int w{};
  int h{};
  float m_fps = 100.0f;
  Window mainWindow{};

public:
  static void Create(Window) noexcept;
  static IVideoContext *GetInstance() noexcept { return instance; }
  VideoContextSDL(Window data) { mainWindow = data; }

  ~VideoContextSDL() noexcept override;

  void setCamera(const Coord &pos) noexcept override { cameraPosition = pos; }

  void setScale(float scale) noexcept override {
    m_scale = scale;
    SDL_SetRenderScale(rend, m_scale, m_scale);
    SDL_Rect viewport{0, 0, (int)(w / m_scale), (int)(h / m_scale)};
    SDL_SetRenderViewport(rend, &viewport);
  }

  int getWidth() const noexcept override { return w; }
  int getHeigt() const noexcept override { return h; }
  float getFps() const noexcept override { return m_fps; }
  bool isVisible(const Coord &obj) const noexcept override {

    const auto posx = obj.x;
    const auto posy = obj.y;

    return (posx >= cameraPosition.x - w / 2.0 / (double)m_scale - 5000) &&
           (posx <= cameraPosition.x + w / 2.0 / (double)m_scale + 5000) &&
           (posy >= cameraPosition.y - h / 2.0 / (double)m_scale - 5000) &&
           (posy <= cameraPosition.y + h / 2.0 / (double)m_scale + 5000);
  }
  inline void clear() noexcept override;
  void delay(size_t ms) const noexcept override;
  void present() noexcept override;
  void draw(const MovableObject *obj, double timeDiff) noexcept override;
  void draw(const StaticObject *obj, double timeDiff) noexcept override;
  void setup() noexcept override;
  static void getPrimaryDisplayResolution(int &w, int &h) {
    auto dmode = SDL_GetCurrentDisplayMode(SDL_GetPrimaryDisplay());
    w = dmode->w;
    h = dmode->h;
  }
};
