#pragma once

#include <atomic>
#include <chrono>
#include <list>
#include <mutex>
#include <vector>

#include "IObject.hpp"
#include "IRenderer.hpp"

class RenderData final {
private:
  mutable std::vector<IObject *> buffer;
  mutable std::mutex mtx;
  mutable std::chrono::time_point<std::chrono::steady_clock> start{};
  mutable std::atomic_bool dataReady{false};

public:
  void GetRenderData(std::vector<IObject *> &, double &timeDiff) const noexcept;
  void PushRenderingData(const std::list<IObject *> &,
                         const IRenderer &) noexcept;
};
