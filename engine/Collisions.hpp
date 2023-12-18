#pragma once

#include <set>
#include <vector>

#include "CppHacks.hpp"
#include "IObject.hpp"
class MovableObject;

#include "Config.hpp"
#include <tuple>
#include <unordered_map>
#include <unordered_set>

struct Coord;
class StaticObject;

constexpr size_t BUCKET_COLS{MAX_COORD / ONE_BUCKET_RESOLUTION};
static_assert(BUCKET_COLS * ONE_BUCKET_RESOLUTION >= MAX_COORD, "");
constexpr size_t NUM_BUCKETS{BUCKET_COLS * BUCKET_COLS};

class Collisions final {
private:
  std::unordered_set<const IObject *> m_bkt[NUM_BUCKETS];

public:
  static Collisions *getInstance() noexcept;
  void update(MovableObject &obj) noexcept;
  void update_static(const StaticObject &obj) noexcept;
  bool checkCollisions(const MovableObject &obj) const noexcept;

private:
  INLINE bool
  collision(const int64_t num, const MovableObject &obj,
            std::unordered_set<const IObject *> &not_collide) const noexcept;
};
