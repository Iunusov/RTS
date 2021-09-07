#pragma once

#include <set>
#include <unordered_map>
#include <vector>

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
  inline size_t getBucketNum(const Coord &tgt) const noexcept;
  inline bool checkRow(const int64_t idx, const Coord &pos,
                       const size_t id) const noexcept;
  inline bool collision(const int64_t num, const Coord &coord,
                        const size_t id) const noexcept;
};
