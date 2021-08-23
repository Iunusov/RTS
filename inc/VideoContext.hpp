#pragma once

#include <SDL.h>

class TestObject;

class VideoContext final {
private:
  SDL_Renderer *rend = nullptr;

private:
  VideoContext(SDL_Renderer *rend) : rend(rend) {}

  static VideoContext *instance;

public:
  static void Create(SDL_Renderer *r) noexcept;
  static VideoContext *GetInstance() noexcept { return instance; }
  void draw(const TestObject &obj) noexcept;
};
