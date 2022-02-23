#pragma once

#include <set>
#include <unordered_map>
#include <vector>

#include "CppHacks.hpp"
#include "IObject.hpp"
class IMovableObject;

#include "Config.hpp"
#include <tuple>
#include <unordered_set>

struct Coord;
class IStaticObject;

class Collisions final {
private:
  std::set<const IObject *> *buckets = nullptr;
  size_t getBucketForObjectID[MAX_COUNT] = {};
  Collisions() noexcept;

public:
  static Collisions *getInstance() noexcept;
  ~Collisions() noexcept { delete[] buckets; }
  void update(const IMovableObject &obj) noexcept;
  void update_static(const IStaticObject &obj) noexcept;
  bool checkCollisions(const IMovableObject &obj) const noexcept;

private:
  INLINE bool
  collision(const int64_t num, const IMovableObject &obj,
            std::unordered_set<const IObject *> &not_collide) const noexcept;
};
