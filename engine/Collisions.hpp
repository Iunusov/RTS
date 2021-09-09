#pragma once

#include <set>
#include <unordered_map>
#include <vector>

#include "CppHacks.hpp"
#include "IObject.hpp"

#include "Config.hpp"

struct Coord;

class Collisions final {
private:
  std::set<IObject *> *buckets = nullptr;
  ssize_t getBucketForObjectID[MAX_COUNT] = {};
  Collisions() noexcept;

public:
  static Collisions *getInstance() noexcept;
  ~Collisions() noexcept { delete[] buckets; }
  void update(IObject &obj) noexcept;
  bool checkCollisions(const Coord &pos, const size_t id) const noexcept;

private:
  INLINE bool collision(const int64_t num, const Coord &coord,
                        const size_t id) const noexcept;
};
