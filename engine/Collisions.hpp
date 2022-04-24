#pragma once

#include <set>
#include <unordered_map>
#include <vector>

#include "CppHacks.hpp"
#include "IObject.hpp"
class IMovableObject;

#include "Config.hpp"
#include <tuple>
#include <unordered_map>
#include <unordered_set>

struct Coord;
class IStaticObject;

constexpr size_t BUCKET_COLS{(MAX_COORD / ONE_BUCKET_RESOLUTION) + 1};
static_assert(BUCKET_COLS * ONE_BUCKET_RESOLUTION >= MAX_COORD, "");
constexpr size_t NUM_BUCKETS{BUCKET_COLS * BUCKET_COLS};

class Collisions final {
private:
  std::set<const IObject *> m_bkt[NUM_BUCKETS];
  std::unordered_map<size_t, size_t> m_bkt_id;

public:
  static Collisions *getInstance() noexcept;
  void update(const IMovableObject &obj) noexcept;
  void update_static(const IStaticObject &obj) noexcept;
  bool checkCollisions(const IMovableObject &obj) const noexcept;

private:
  INLINE bool
  collision(const int64_t num, const IMovableObject &obj,
            std::unordered_set<const IObject *> &not_collide) const noexcept;
};
