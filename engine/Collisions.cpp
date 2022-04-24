#include "Collisions.hpp"

#include "IMovableObject.hpp"
#include "IStaticObject.hpp"

#include "Config.hpp"
#include "Coord.hpp"

namespace {

inline size_t getBucketNum(const Coord &tgt) noexcept {
  const int64_t result =
      (((int64_t)(tgt.x)) / ONE_BUCKET_RESOLUTION) +
      (((int64_t)(tgt.y)) / ONE_BUCKET_RESOLUTION) * BUCKET_COLS;
  return result >= 0 ? (size_t)result : NUM_BUCKETS;
}
} // namespace

Collisions *Collisions::getInstance() noexcept {
  static Collisions instance;
  return &instance;
}

void Collisions::update(const IMovableObject &obj) noexcept {
  const auto newBktId{getBucketNum(obj.getPosition())};
  const auto id{obj.getId()};

  const auto bktId = m_bkt_id.find(id);

  if (bktId != m_bkt_id.end()) {
    if (bktId->second == newBktId) {
      return;
    } else {
      m_bkt[bktId->second].erase(&obj);
    }
  }
  m_bkt[newBktId].emplace(&obj);
  m_bkt_id[id] = newBktId;
}

void Collisions::update_static(const IStaticObject &obj) noexcept {

  for (const auto &p : obj.getPoints()) {
    const auto pos = obj.getPosition() + p -
                     Coord{obj.getWidth() / 2.0, obj.getHeight() / 2.0};
    const auto bkt = getBucketNum(pos);
    if (bkt >= NUM_BUCKETS) {
      continue;
    }
    m_bkt[bkt].emplace(&obj);
  }
}

bool Collisions::checkCollisions(const IMovableObject &obj) const noexcept {
  if (obj.getPosition().x < 0 || obj.getPosition().x >= MAX_COORD ||
      obj.getPosition().y < 0 || obj.getPosition().y >= MAX_COORD ||
      getBucketNum(obj.getPosition()) >= NUM_BUCKETS) {
    return true;
  }

  static std::unordered_set<const IObject *> noCollisions;
  noCollisions.clear();

  const auto bkt{getBucketNum(obj.getPosition())};
#define checkRow(idx, obj)                                                     \
  (bool{(collision(int64_t(idx - 1), obj, noCollisions) ||                     \
         collision(int64_t(idx), obj, noCollisions) ||                         \
         collision(int64_t(idx + 1), obj, noCollisions))})

  return checkRow(bkt - BUCKET_COLS, obj) || checkRow(bkt, obj) ||
         checkRow(bkt + BUCKET_COLS, obj);
}

bool Collisions::collision(
    const int64_t num, const IMovableObject &obj,
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
