#include "Collisions.hpp"

#include "Config.hpp"
#include "Coord.hpp"

namespace {
constexpr size_t THRESHOLD{250};
constexpr size_t ONE_BUCKET_RESOLUTION{222};
constexpr size_t BUCKET_COLS{(MAX_COORD / ONE_BUCKET_RESOLUTION) + 1};
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

Collisions::Collisions() noexcept { buckets.resize(NUM_BUCKETS); }

void Collisions::update(IObject &obj) noexcept {
  const auto newBktId{getBucketNum(obj.getPosition())};
  const auto id{obj.getId()};
  const auto bktIter{getBucketForObjectID.find(id)};
  if (bktIter != getBucketForObjectID.end())
    [[likely]] {
      if (bktIter->second == newBktId)
        [[likely]] { return; }
      else
        [[unlikely]] { buckets.at(bktIter->second).erase(&obj); }
    }
  buckets.at(newBktId).insert(&obj);
  getBucketForObjectID[id] = newBktId;
}

#define checkRow(idx, pos, id)                                                 \
  (bool{                                                                       \
    (collision((idx - 1), pos, id) || collision((idx), pos, id) ||             \
     collision((idx + 1), pos, id))                                            \
  })
bool Collisions::checkCollisions(const Coord &pos, const size_t id) const
    noexcept {
  if (pos.x > 0 && pos.x < MAX_COORD && pos.y > 0 && pos.y < MAX_COORD)
    [[likely]] {
      const auto bkt{getBucketNum(pos)};
      return checkRow(bkt - BUCKET_COLS, pos, id) || checkRow(bkt, pos, id) ||
             checkRow(bkt + BUCKET_COLS, pos, id);
    }
  else
    [[unlikely]] { return true; }
}

bool Collisions::collision(const int64_t num, const Coord &coord,
                           const size_t id) const noexcept {
  if ((num >= 0) && (num < NUM_BUCKETS))
    [[likely]] {
      const auto &bkt = buckets.at(num);
      for (const auto &l : bkt) {
        [[likely]] if ((id != l->getId()) &&
                       (coord.distance(l->getPosition()) < THRESHOLD)) {
          return true;
        }
      }
      return false;
    }
  else
    [[unlikely]] { return false; }
}
