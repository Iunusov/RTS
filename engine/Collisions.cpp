#include "Collisions.hpp"

#include "Config.hpp"
#include "Coord.hpp"

namespace {
constexpr size_t THRESHOLD{250};

constexpr size_t BUCKET_COLS{(MAX_COORD / ONE_BUCKET_RESOLUTION) + 1};
static_assert(BUCKET_COLS * ONE_BUCKET_RESOLUTION >= MAX_COORD, "");
constexpr size_t NUM_BUCKETS{BUCKET_COLS * BUCKET_COLS};

inline size_t getBucketNum(const Coord &tgt) noexcept {
  return ((((size_t)(tgt.x)) / ONE_BUCKET_RESOLUTION) +
          (((size_t)(tgt.y)) / ONE_BUCKET_RESOLUTION) * BUCKET_COLS);
}
} // namespace

Collisions *Collisions::getInstance() noexcept {
  static Collisions instance;
  return &instance;
}

Collisions::Collisions() noexcept {
  buckets = new std::set<const IObject *>[NUM_BUCKETS];
  for (size_t i(0); i < MAX_COUNT; ++i) {
    getBucketForObjectID[i] = SIZE_MAX;
  }
}

void Collisions::update(const IObject &obj) noexcept {
  const auto newBktId{getBucketNum(obj.getPosition())};
  const auto id{obj.getId()};

  const auto bktId = getBucketForObjectID[(id)];

  if (bktId != SIZE_MAX) {
    if (bktId == newBktId) {
      return;
    } else {
      buckets[bktId].erase(&obj);
    }
  }
  buckets[newBktId].emplace(&obj);
  getBucketForObjectID[id] = newBktId;
}

bool Collisions::checkCollisions(const Coord &pos,
                                 const size_t id) const noexcept {
  if (pos.x < 0 || pos.x >= MAX_COORD || pos.y < 0 || pos.y >= MAX_COORD ||
      getBucketNum(pos) > NUM_BUCKETS) {
    return true;
  }

  const auto bkt{getBucketNum(pos)};
#define checkRow(idx, pos, id)                                                 \
  (bool{(collision(int64_t(idx - 1), pos, id) ||                               \
         collision(int64_t(idx), pos, id) ||                                   \
         collision(int64_t(idx + 1), pos, id))})

  return checkRow(bkt - BUCKET_COLS, pos, id) || checkRow(bkt, pos, id) ||
         checkRow(bkt + BUCKET_COLS, pos, id);
}

bool Collisions::collision(const int64_t num, const Coord &coord,
                           const size_t id) const noexcept {
  if (num < 0 || (size_t)num >= NUM_BUCKETS) {
    return false;
  }

  const auto &bkt = buckets[num];
  for (const auto &l : bkt) {
    if ((id != l->getId()) && (coord.distance(l->getPosition()) < THRESHOLD)) {
      return true;
    }
  }
  return false;
}
