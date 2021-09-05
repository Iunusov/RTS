#include "RenderData.hpp"

#include <atomic>
#include <chrono>
#include <cmath>
#include <mutex>
#include <vector>

#include "Config.hpp"
#include "Coord.hpp"
#include "IMovableObject.hpp"
#include "Math.hpp"

namespace {

std::vector<IObject *> buffer;
std::mutex mtx;
auto start = std::chrono::steady_clock::now();
std::atomic_bool dataReady{false};

} // namespace

namespace RenderData {
void GetRenderData(std::vector<IObject *> &data, double &timeDiff) noexcept {
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

  timeDiff = (double)gpuMS / (double)MODEL_CYCLE_TIME_MS;
  dataReady = false;
}

void PushRenderingData(const std::list<IObject *> &data,
                       const Renderer2D &rend) noexcept {
  if (dataReady) {
    return;
  }
  mtx.lock();

  for (size_t i(0); i < buffer.size(); ++i) {
    delete (buffer[i]);
  }
  buffer.resize(0);
  for (const auto &obj : data) {
    if (rend.isVisible(*obj)) {
      buffer.emplace_back(obj->clone());
    }
  }
  start = std::chrono::steady_clock::now();
  mtx.unlock();
  dataReady = true;
}

} // namespace RenderData
