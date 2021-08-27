#pragma once

#include <SDL.h>

struct Coord;
class TestObject;

class VideoContext final {
private:
  static VideoContext *instance;
  SDL_Renderer *rend = nullptr;
  VideoContext(SDL_Renderer *rend) noexcept : rend(rend) {}

public:
  static void Create(SDL_Renderer *r) noexcept;
  static VideoContext *GetInstance() noexcept { return instance; }
  void draw(const Coord &camera, const TestObject &obj) noexcept;
};
