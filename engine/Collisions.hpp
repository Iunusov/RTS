#pragma once

#include <set>
#include <unordered_map>
#include <vector>

#include "CppHacks.hpp"
#include "IObject.hpp"
class IMovableObject;

#include "Config.hpp"
#include <tuple>

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
  INLINE bool collision(const int64_t num,
                        const IMovableObject &obj) const noexcept;
};
