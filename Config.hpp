#pragma once
#include <stddef.h>
constexpr const auto MAX_COORD{500000};
constexpr size_t ONE_BUCKET_RESOLUTION{200};
constexpr const auto MODEL_EXECUTE_PER_SECOND{5};

constexpr const auto MODEL_CYCLE_TIME_MS{1000 / MODEL_EXECUTE_PER_SECOND};
static_assert(MODEL_EXECUTE_PER_SECOND * MODEL_CYCLE_TIME_MS == 1000, "");
