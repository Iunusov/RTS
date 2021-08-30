#pragma once

constexpr const size_t MODEL_EXECUTE_PER_SECOND{60};
constexpr const size_t MODEL_CYCLE_TIME_MICROSECONDS{
    (size_t)(1000000.0 / MODEL_EXECUTE_PER_SECOND)};
