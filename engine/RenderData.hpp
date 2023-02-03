#pragma once

#include <atomic>
#include <chrono>
#include <vector>

#include "IObject.hpp"
#include "IRenderer.hpp"

class RenderData final {
private:
  mutable std::vector<IObject *> buffer;
  std::chrono::time_point<std::chrono::steady_clock> timestamp{};
  mutable std::atomic_bool dataReady{false};

public:
  void GetRenderData(
      std::vector<IObject *> &,
      std::chrono::time_point<std::chrono::steady_clock> &) const noexcept;
  void PushRenderingData(const std::vector<IObject *> &,
                         const IRenderer &) noexcept;
};
