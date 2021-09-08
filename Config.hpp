#pragma once

constexpr const auto MAX_COORD{50000};
constexpr size_t ONE_BUCKET_RESOLUTION{333};
constexpr const auto MODEL_EXECUTE_PER_SECOND{10};

constexpr const auto MODEL_CYCLE_TIME_MS{1000 / MODEL_EXECUTE_PER_SECOND};
static_assert(MODEL_EXECUTE_PER_SECOND * MODEL_CYCLE_TIME_MS == 1000, "");
