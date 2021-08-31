#pragma once

#include <list>

#include "IObject.hpp"

namespace RenderData {

void GetRenderData(std::list<IObject *> &data) noexcept;

void PushRenderingData(const std::list<IObject *> &data) noexcept;

} // namespace RenderData
