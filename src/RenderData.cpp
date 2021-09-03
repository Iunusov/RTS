#include "RenderData.hpp"
#include "Coord.hpp"
#include <chrono>
#include <cmath>
#include <mutex>
#include <vector>

#include "Config.hpp"

#include "IMovableObject.hpp"

#include <atomic>

namespace {

std::vector<IObject *> buffer;
std::mutex mtx;
auto start = std::chrono::steady_clock::now();
std::atomic_bool dataReady{false};

constexpr inline long double lerp(const long double a, const long double b,
                             const long double t) noexcept {
  return (a + t * (b - a));
}

} // namespace

namespace RenderData {
void GetRenderData(std::vector<IObject *> &data) noexcept {
  for (size_t i(0); i < data.size(); ++i) {
    delete (data[i]);
  }
  data.clear();
  mtx.lock();
  data.reserve(buffer.size());
  for (const auto &obj : buffer) {
    data.emplace_back(obj->clone());
  }

  auto gpuMS = std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::steady_clock::now() - start)
                   .count();
  mtx.unlock();

  const auto t = (double)gpuMS / (double)MODEL_CYCLE_TIME_MS;

  for (auto &o : data) {
    o->setPosition(
        Coord{(lerp(dynamic_cast<IMovableObject *>(o)->previousPosition.x,
                    o->getPosition().x, t)),
              (lerp(dynamic_cast<IMovableObject *>(o)->previousPosition.y,
                    o->getPosition().y, t))});

    auto *movable = dynamic_cast<IMovableObject *>(o);

    if (movable) {
      movable->fire_angle =
          (double)lerp(movable->prev_fire_angle, movable->fire_angle, t);

      movable->setHeading(
          (double)lerp(movable->prev_heading, movable->heading, t));
    }
  }
  dataReady = false;
}

void PushRenderingData(const std::list<IObject *> &data,
                       const Coord &cam) noexcept {
  if (dataReady) {
    return;
  }
  mtx.lock();

  for (size_t i(0); i < buffer.size(); ++i) {
    delete (buffer[i]);
  }
  buffer.resize(0);
  for (const auto &obj : data) {
    if (obj->getPosition().x > cam.x + 1920 ||
        obj->getPosition().y > cam.y + 1080 ||
        obj->getPosition().x < cam.x - 400 ||
        obj->getPosition().y < cam.y - 400) {
      continue;
    }
    buffer.emplace_back(obj->clone());
  }
  start = std::chrono::steady_clock::now();
  mtx.unlock();
  dataReady = true;
}

} // namespace RenderData
