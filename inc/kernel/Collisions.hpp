#pragma once

#include <list>
#include <set>
#include <unordered_map>
#include <vector>

#include "Coord.hpp"
#include "IObject.hpp"

#include "Config.hpp"

namespace {
constexpr size_t BUCKET_DIMENSION{500};
constexpr size_t BUCKET_NUM{(MAX_COORD / BUCKET_DIMENSION) + 1};
constexpr size_t BUCKET_COUNT{BUCKET_NUM * BUCKET_NUM};
} // namespace
class Collisions {
private:
  std::vector<std::set<IObject *>> bucket{};
  std::unordered_map<size_t, size_t> objBucket;

private:
  Collisions() noexcept {
    bucket.reserve(BUCKET_COUNT);
    bucket.resize(BUCKET_COUNT);
  }

public:
  static Collisions *getInstance() {
    static Collisions instance;
    return &instance;
  }

  inline size_t getBucketNum(const Coord &tgt) const noexcept {
    return (((size_t)(tgt.x)) / BUCKET_DIMENSION) +
           (((size_t)(tgt.y)) / BUCKET_DIMENSION) * BUCKET_NUM;
  }

  inline void update(IObject &obj) noexcept {
    const auto calcBucketID{getBucketNum(obj.getPosition())};

    {
      const auto bucketID{objBucket.find(obj.getId())};
      if (bucketID != objBucket.end()) {
        if (bucketID->second == calcBucketID) {
          return;
        }
        bucket.at(bucketID->second).erase(&obj);
      }
    }

    bucket.at(calcBucketID).insert(&obj);
    objBucket[obj.getId()] = calcBucketID;
  }

  inline bool isInCollision(int64_t bucketNum, const Coord &coord,
                            const size_t id) const noexcept {
    if (bucketNum < 0 || (size_t)bucketNum >= bucket.size()) {
      return false;
    }

    const auto &bkt = bucket.at(bucketNum);
    for (auto &l : bkt) {
      if (id == l->getId()) {
        continue;
      }

      const auto distance = coord.distance(l->getPosition());

      if (distance < 300) {
        return true;
      }
    }

    return false;
  }

  inline bool checkCollisions(const Coord &obj, size_t id) const noexcept {
    const auto bucketNum = getBucketNum(obj);
    return isInCollision(bucketNum, obj, id) ||

           isInCollision(bucketNum + 1, obj, id) ||
           isInCollision(bucketNum + BUCKET_NUM - 1, obj, id) ||
           isInCollision(bucketNum + BUCKET_NUM, obj, id) ||
           isInCollision(bucketNum + BUCKET_NUM + 1, obj, id) ||

           isInCollision(bucketNum - 1, obj, id) ||
           isInCollision(bucketNum - BUCKET_NUM - 1, obj, id) ||
           isInCollision(bucketNum - BUCKET_NUM, obj, id) ||
           isInCollision(bucketNum - BUCKET_NUM + 1, obj, id);
  }
};
