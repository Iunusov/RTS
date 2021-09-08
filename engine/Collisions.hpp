#pragma once

#include <set>
#include <unordered_map>
#include <vector>

#include "CppHacks.hpp"
#include "IObject.hpp"

struct Coord;

class Collisions final {
private:
  std::unordered_map<size_t, size_t> getBucketForObjectID;
  std::vector<std::set<IObject *>> buckets;
  Collisions() noexcept;

public:
  static Collisions *getInstance() noexcept;
  void update(IObject &obj) noexcept;
  bool checkCollisions(const Coord &pos, const size_t id) const noexcept;

private:
  INLINE bool collision(const int64_t num, const Coord &coord,
                        const size_t id) const noexcept;
};
