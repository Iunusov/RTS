#pragma once

#include "Coord.hpp"
#include "IObject.hpp"
#include "IVideoContext.hpp"

#include <SDL.h>

class VideoContextSDL final : public IVideoContext {
private:
  static IVideoContext *instance;
  SDL_Renderer *rend = nullptr;
  Coord cameraPosition;
  SDL_Window *win = nullptr;

public:
  static void Create() noexcept;
  static IVideoContext *GetInstance() noexcept { return instance; }

  ~VideoContextSDL() noexcept override;

  void setCamera(const Coord &pos) noexcept override { cameraPosition = pos; }
  void draw(const Map &obj) noexcept override;
  void clear() noexcept override;
  void delay(size_t ms) const noexcept override;
  void present() noexcept override;
  void draw(const IObject *obj) noexcept override;
  void setup() noexcept override;
};
