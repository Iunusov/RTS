#pragma once

#include <list>
#include <vector>

#include "IObject.hpp"
#include "Renderer.hpp"

namespace RenderData {

void GetRenderData(std::vector<IObject *> &data, double &timeDiff) noexcept;

void PushRenderingData(const std::list<IObject *> &data,
                       const Renderer2D &) noexcept;

} // namespace RenderData
