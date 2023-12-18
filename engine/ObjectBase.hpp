#pragma once

#include "Coord.hpp"
#include "CppHacks.hpp"
#include "ICommand.hpp"
#include "IObject.hpp"

#include "CommandQueue.hpp"

#include <iostream>
#include <optional>

namespace {
size_t getGlobalId() noexcept {
  static size_t id{0};
  return ++id;
}
} // namespace

class ObjectBase : public IObject {
  friend class Render2D;

private:
  std::optional<size_t> coll_bkt_id_{};

protected:
  const size_t id{getGlobalId()};

  int64_t health = 100;

  Coord position{};
  double heading{};

  CommandQueue cmds;

public:
  Coord previousPosition{};
  double previousHeading{};
  void teleportTo(const Coord &pos) NCNOF {
    previousPosition = pos;
    position = pos;
  }

  size_t getId() CNOF { return id; }
  int64_t getHealth() CNOF { return health; }
  const Coord &getPositionRef() CNOF { return position; }
  void setPosition(const Coord &pos) NCNOF { position = pos; }
  double getHeading() CNOF { return heading; }
  void setHeading(double angle) NCNOF { heading = angle; };

  void execute() noexcept override;
  void acceptCommand(const ICommand &command) NCNOF;

  std::optional<size_t> getCollisionsBucketID() CNOF { return coll_bkt_id_; }
  void setCollisionsBucketID(size_t b_id) NCNOF { coll_bkt_id_ = b_id; }
};
