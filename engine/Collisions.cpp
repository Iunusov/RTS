#include "Collisions.hpp"

#include "IMovableObject.hpp"
#include "IStaticObject.hpp"

#include "Config.hpp"
#include "Coord.hpp"

namespace {
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

void Collisions::update(const IMovableObject &obj) noexcept {
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

void Collisions::update_static(const IStaticObject &obj) noexcept {

  for (const auto &p : obj.getPoints()) {
    const auto pos = obj.getPosition() + p -
                     Coord{obj.getWidth() / 2.0, obj.getHeight() / 2.0};
    const auto bkt = getBucketNum(pos);
    if (bkt >= NUM_BUCKETS) {
      std::cout << obj.getPosition().x << " " << obj.getPosition().y
                << std::endl;
      std::cout << p.x << " " << p.y << std::endl;
      std::cout << bkt << std::endl;
      exit(1);
    }
    buckets[bkt].emplace(&obj);
  }
}

bool Collisions::checkCollisions(const IMovableObject &obj) const noexcept {
  if (obj.getPosition().x < 0 || obj.getPosition().x >= MAX_COORD ||
      obj.getPosition().y < 0 || obj.getPosition().y >= MAX_COORD ||
      getBucketNum(obj.getPosition()) > NUM_BUCKETS) {
    return true;
  }

  const auto bkt{getBucketNum(obj.getPosition())};
#define checkRow(idx, obj)                                                     \
  (bool{(collision(int64_t(idx - 1), obj) || collision(int64_t(idx), obj) ||   \
         collision(int64_t(idx + 1), obj))})

  return checkRow(bkt - BUCKET_COLS, obj) || checkRow(bkt, obj) ||
         checkRow(bkt + BUCKET_COLS, obj);
}

bool Collisions::collision(const int64_t num,
                           const IMovableObject &obj) const noexcept {
  if (num < 0 || (size_t)num >= NUM_BUCKETS) {
    return false;
  }

  const auto &bkt = buckets[num];
  for (const auto &l : bkt) {
    if (l->collide(obj)) {
      return true;
    }
  }

  return false;
}
