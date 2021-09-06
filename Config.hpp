#pragma once

constexpr const auto MODEL_EXECUTE_PER_SECOND{10};

constexpr const auto MODEL_CYCLE_TIME_MS{1000 / MODEL_EXECUTE_PER_SECOND};
constexpr const auto MAX_COORD{300000};
static_assert(MODEL_EXECUTE_PER_SECOND * MODEL_CYCLE_TIME_MS == 1000, "");
