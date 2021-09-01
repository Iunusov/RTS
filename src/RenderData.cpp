#include "RenderData.hpp"
#include "Coord.hpp"
#include <chrono>
#include <cmath>
#include <mutex>

#include "Config.hpp"

#include "IMovableObject.hpp"

namespace {

std::list<IObject *> buffer;
std::mutex mtx;
auto start = std::chrono::steady_clock::now();

constexpr inline long double lerp(const long double a, const long double b,
                                  const long double t) noexcept {
  return (a + t * (b - a));
}

} // namespace

namespace RenderData {
void GetRenderData(std::list<IObject *> &data) noexcept {
  const std::lock_guard<std::mutex> guard(mtx);
  for (auto &v : data) {
    delete (v);
  }
  data.clear();
  for (const auto &obj : buffer) {
    data.emplace_back(obj->clone());
  }

  const auto gpuMS = std::chrono::duration_cast<std::chrono::milliseconds>(
                         std::chrono::steady_clock::now() - start)
                         .count();

  const auto t = (double)gpuMS / (double)MODEL_CYCLE_TIME_MS;

  for (auto &o : data) {
    o->setPosition(Coord{(int64_t)std::round(lerp((double)o->getPreviousPosition().x,
                                                  (double)o->getPosition().x, t)),
                         (int64_t)std::round(lerp((double)o->getPreviousPosition().y,
                                                  (double)o->getPosition().y, t))});

    auto *movable = dynamic_cast<IMovableObject *>(o);

    if (movable) {
      movable->fire_angle = (double)
          lerp(movable->prev_fire_angle, movable->fire_angle, t);
    }
  }
}

void PushRenderingData(const std::list<IObject *> &data) noexcept {
  const std::lock_guard<std::mutex> guard(mtx);

  for (auto *v : buffer) {
    delete (v);
  }
  buffer.clear();
  for (const auto &obj : data) {
    buffer.emplace_back(obj->clone());
  }
  start = std::chrono::steady_clock::now();
}

} // namespace RenderData
