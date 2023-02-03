#include "RenderData.hpp"

#include <chrono>

#include "Config.hpp"

#include <cassert>

void RenderData::GetRenderData(
    std::vector<IObject *> &data,
    std::chrono::time_point<std::chrono::steady_clock> &ts) const noexcept {
  if (!dataReady.load(std::memory_order_acquire)) {
    return;
  }
  assert (buffer.size() > 0);
  for (size_t i(0); i < data.size(); ++i) {
    delete (data[i]);
  }
  data.clear();
  data.reserve(buffer.size());
  for (const auto &obj : buffer) {
    data.emplace_back(obj->clone());
  }
  for (size_t i(0); i < buffer.size(); ++i) {
    delete (buffer[i]);
  }
  buffer.clear();

  ts = timestamp;
  dataReady.store(false, std::memory_order_release);
}

void RenderData::PushRenderingData(const std::vector<IObject *> &data,
                                   const IRenderer &renderer) noexcept {
  if (dataReady.load(std::memory_order_acquire)) {
    return;
  }
  assert (buffer.size() == 0);
  for (const auto &obj : data) {
    if (!renderer.isVisible(*obj)) {
      continue;
    }
    buffer.emplace_back(obj->clone());
  }
  timestamp = std::chrono::steady_clock::now();
  dataReady.store(true, std::memory_order_release);
}
