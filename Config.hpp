#pragma once
#include <stddef.h>
constexpr const size_t MAX_COORD{1000000};
constexpr size_t ONE_BUCKET_RESOLUTION{1000};
constexpr const size_t MODEL_EXECUTE_PER_SECOND{5};

constexpr const size_t MODEL_CYCLE_TIME_MS{1000 / MODEL_EXECUTE_PER_SECOND};
static_assert(MODEL_EXECUTE_PER_SECOND * MODEL_CYCLE_TIME_MS == 1000, "");

constexpr const float SCALE_STEP{0.15f};
