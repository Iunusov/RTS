#include "RenderData.hpp"

#include <chrono>

#include "Config.hpp"

void RenderData::GetRenderData(std::vector<IObject *> &data,
                               double &timeDiff) const noexcept {
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

void RenderData::PushRenderingData(const std::list<IObject *> &data,
                                   const IRenderer &renderer) noexcept {
  if (dataReady) {
    return;
  }
  mtx.lock();

  for (size_t i(0); i < buffer.size(); ++i) {
    delete (buffer[i]);
  }
  buffer.resize(0);
  for (const auto &obj : data) {
    if (!renderer.isVisible(*obj)) {
      continue;
    }
    buffer.emplace_back(obj->clone());
  }
  start = std::chrono::steady_clock::now();
  mtx.unlock();
  dataReady = true;
}
