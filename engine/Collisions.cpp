#include "Collisions.hpp"

#include "MovableObject.hpp"
#include "StaticObject.hpp"

#include "Config.hpp"
#include "Coord.hpp"

#include <cassert>
#include <cmath>

namespace {

inline int64_t getBucketNum(const Coord &tgt) noexcept {
  return ((int64_t)tgt.x / (int64_t)ONE_BUCKET_RESOLUTION) +
         ((int64_t)tgt.y / (int64_t)ONE_BUCKET_RESOLUTION) *
             (int64_t)BUCKET_COLS;
}
} // namespace

Collisions *Collisions::getInstance() noexcept {
  static Collisions instance;
  return &instance;
}

void Collisions::update(MovableObject &obj) noexcept {
  const int64_t newBktId{getBucketNum(obj.getPositionRef())};
  if (newBktId < 0) {
    return;
  }
  const auto bkt_id{obj.getCollisionsBucketID()};

  if (bkt_id) {
    if (*bkt_id == (size_t)newBktId) {
      return;
    } else {
      m_bkt[*bkt_id].erase(&obj);
    }
  }
  m_bkt[newBktId].emplace(&obj);
  obj.setCollisionsBucketID((size_t)newBktId);
}

void Collisions::update_static(const StaticObject &obj) noexcept {

  for (const auto &p : obj.getPoints()) {
    const auto pos = obj.getPositionRef() + p -
                     Coord{obj.getWidth() / 2.0, obj.getHeight() / 2.0};
    const int64_t bkt = getBucketNum(pos);
    if (bkt < 0 || (size_t)bkt >= NUM_BUCKETS) {
      continue;
    }
    m_bkt[bkt].emplace(&obj);
  }
}

bool Collisions::checkCollisions(const MovableObject &obj) const noexcept {
  const auto &pos = obj.getPositionRef();
  if (pos.x < 0 || pos.x >= MAX_COORD || pos.y < 0 || pos.y >= MAX_COORD ||
      (size_t)getBucketNum(pos) >= NUM_BUCKETS) {
    return true;
  }

  static std::unordered_set<const IObject *> noCollisions;
  noCollisions.clear();

  const int64_t bkt{getBucketNum(obj.getPositionRef())};
#define checkRow(idx, obj)                                                     \
  (bool{(collision(idx, obj, noCollisions) ||                                  \
         collision(idx - 1, obj, noCollisions) ||                              \
         collision(idx + 1, obj, noCollisions))})

  return checkRow(bkt, obj) || checkRow(bkt - (int64_t)BUCKET_COLS, obj) ||
         checkRow(bkt + (int64_t)BUCKET_COLS, obj);
}

bool Collisions::collision(
    const int64_t num, const MovableObject &obj,
    std::unordered_set<const IObject *> &noCollisions) const noexcept {
  if (num < 0 || (size_t)num >= NUM_BUCKETS) {
    return false;
  }

  const auto &bkt = m_bkt[num];
  for (const auto &l : bkt) {
    if (noCollisions.count(l)) {
      continue;
    }
    if (l->collide(obj)) {
      return true;
    } else if (!l->isMovable()) {
      noCollisions.emplace(l);
    }
  }

  return false;
}
