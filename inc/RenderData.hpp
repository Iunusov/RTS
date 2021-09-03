#pragma once

#include <list>
#include <vector>

#include "IObject.hpp"

namespace RenderData {

void GetRenderData(std::vector<IObject *> &data) noexcept;

void PushRenderingData(const std::list<IObject *> &data,
                       const Coord &) noexcept;

} // namespace RenderData
