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
  buckets = new std::set<IObject *>[NUM_BUCKETS];
  for (size_t i(0); i < MAX_COUNT; ++i) {
    getBucketForObjectID[i] = -1;
  }
}

void Collisions::update(IObject &obj) noexcept {
  const auto newBktId{getBucketNum(obj.getPosition())};
  const auto id{obj.getId()};

  const auto bktId = getBucketForObjectID[(id)];

  if (bktId != -1)
    LIKELY {
      if ((size_t)bktId == newBktId)
        LIKELY { return; }
      else
        UNLIKELY { buckets[bktId].erase(&obj); }
    }
  buckets[newBktId].emplace(&obj);
  getBucketForObjectID[id] = newBktId;
}

#define checkRow(idx, pos, id)                                                 \
  (bool{(collision((idx - 1), pos, id) || collision((idx), pos, id) ||         \
         collision((idx + 1), pos, id))})

bool Collisions::checkCollisions(const Coord &pos,
                                 const size_t id) const noexcept {
  if (pos.x > 0 && pos.x < MAX_COORD && pos.y > 0 && pos.y < MAX_COORD)
    LIKELY {
      const auto bkt{getBucketNum(pos)};
      return checkRow(bkt - BUCKET_COLS, pos, id) || checkRow(bkt, pos, id) ||
             checkRow(bkt + BUCKET_COLS, pos, id);
    }
  else
    UNLIKELY { return true; }
}

bool Collisions::collision(const int64_t num, const Coord &coord,
                           const size_t id) const noexcept {
  if ((num >= 0) && ((size_t)num < NUM_BUCKETS))
    LIKELY {
      const auto &bkt = buckets[num];
      for (const auto &l : bkt) {
        LIKELY if ((id != l->getId()) &&
                   (coord.distance(l->getPosition()) < THRESHOLD)) {
          return true;
        }
      }
      return false;
    }
  else
    UNLIKELY { return false; }
}
