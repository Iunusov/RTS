#include "RenderData.hpp"
#include <atomic>

namespace {

std::list<IObject *> buffer;

std::atomic_bool data_ready{false};

} // namespace

namespace RenderData {
void GetRenderData(std::list<IObject *> &data) noexcept {
  if (!data_ready) {
    return;
  }
  for (auto &v : data) {
    delete (v);
  }
  data.clear();
  for (const auto &obj : buffer) {
    data.emplace_back(obj->clone());
  }
  data_ready = false;
}

void PushRenderingData(const std::list<IObject *> &data) noexcept {
  if (data_ready) {
    return;
  }
  for (auto *v : buffer) {
    delete (v);
  }
  buffer.clear();
  for (const auto &obj : data) {
    buffer.emplace_back(obj->clone());
  }
  data_ready = true;
}

} // namespace RenderData
