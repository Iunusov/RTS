#pragma once

#include <unordered_map>

#include "Cloneable.hpp"
#include "Coord.hpp"
#include "CppHacks.hpp"
#include "ICommand.hpp"
#include "IObject.hpp"
#include "Math.hpp"

#include "CommandQueue.hpp"

#include <iostream>

namespace {
size_t getGlobalId() noexcept {
  static size_t id{0};
  return ++id;
}
} // namespace

class ObjectBase : public IObject {
private:
  const size_t id{getGlobalId()};

  int64_t health = 100;

  Coord position{};
  double heading{};

  Coord previousPosition{};
  double previousHeading{};
  CommandQueue cmds;

public:
  void approx(double timeDiff) noexcept override {
    setPosition(Coord{Math::lerp(previousPosition.x, position.x, timeDiff),
                      Math::lerp(previousPosition.y, position.y, timeDiff)});

    setHeading(Math::lerp(previousHeading, heading, timeDiff));
  }

  void teleportTo(const Coord &pos) NCNOF {
    previousPosition = pos;
    position = pos;
  }

  size_t getId() CNOF { return id; }
  virtual int64_t getHealth() CNOF { return health; }
  Coord getPosition() CNOF { return position; }
  void setPosition(const Coord &pos) NCNOF { position = pos; }
  double getHeading() CNOF { return heading; }
  void setHeading(double angle) NCNOF { heading = angle; };

  void execute() noexcept override;
  void acceptCommand(const ICommand &command) NCNOF;

  bool isVisible(const IVideoContext &ctx) CNOF;
};
