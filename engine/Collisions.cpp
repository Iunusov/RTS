#include "Collisions.hpp"

#include "Config.hpp"
#include "Coord.hpp"

namespace {
const size_t ONE_BUCKET_RESOLUTION{500};
const size_t BUCKET_COLS{(MAX_COORD / ONE_BUCKET_RESOLUTION) + 1};
const size_t NUM_BUCKETS{BUCKET_COLS * BUCKET_COLS};
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
  if (bktIter != getBucketForObjectID.end()) {
    if (bktIter->second == newBktId) {
      return;
    }
    buckets.at(bktIter->second).erase(&obj);
  }
  buckets.at(newBktId).insert(&obj);
  getBucketForObjectID[id] = newBktId;
}

bool Collisions::checkCollisions(const Coord &pos, const size_t id) const
    noexcept {
  if (pos.x <= 0 || pos.x >= MAX_COORD || pos.y <= 0 || pos.y >= MAX_COORD) {
    return true;
  }
  const auto bkt{getBucketNum(pos)};
  return checkRow(bkt - BUCKET_COLS, pos, id) || checkRow(bkt, pos, id) ||
         checkRow(bkt + BUCKET_COLS, pos, id);
}

size_t Collisions::getBucketNum(const Coord &tgt) const noexcept {
  return ((((size_t)(tgt.x)) / ONE_BUCKET_RESOLUTION) +
          (((size_t)(tgt.y)) / ONE_BUCKET_RESOLUTION) * BUCKET_COLS);
}

bool Collisions::checkRow(const int64_t idx, const Coord &pos,
                          const size_t id) const noexcept {
  return collision(idx - 1, pos, id) || collision(idx, pos, id) ||
         collision(idx + 1, pos, id);
}

bool Collisions::collision(const int64_t num, const Coord &coord,
                           const size_t id) const noexcept {
  if ((num >= 0) && (num < NUM_BUCKETS)) {
    const auto &bkt = buckets.at(num);
    for (auto &l : bkt) {
      if ((id != l->getId()) && (coord.distance(l->getPosition()) < 300)) {
        return true;
      }
    }
  }
  return false;
}
