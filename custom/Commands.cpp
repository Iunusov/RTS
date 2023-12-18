#include <ctime>
#include <functional>
#include <random>

#include "CommandBase.hpp"
#include "Config.hpp"
#include "MovableObject.hpp"
#include "ObjectBase.hpp"

namespace IdleCommands {
class CommandIdle final {
  bool f = true;

public:
  bool execute(IObject &o) {
    static std::mt19937 rng((unsigned int)time(NULL));
    static std::uniform_int_distribution<int64_t> gen(0, 359);
    double diff = (double)360.0 / (double)MODEL_EXECUTE_PER_SECOND;

    MovableObject &obj = dynamic_cast<MovableObject &>(o);
    if (f) {
      obj.fire_angle += diff / 4.0;
    } else {
      obj.fire_angle -= diff / 4.0;
    }

    if (obj.fire_angle >= 360.0) {
      f = false;
    }
    if (obj.fire_angle <= 0) {
      f = true;
    }
    return false;
  }
};

const ICommand *cmd{new CommandBase<CommandIdle>(ICommand::Priority::IDLE)};
} // namespace IdleCommands

namespace CommandMove {
class CommandMove final {
  bool f = true;
  Coord prev{};

public:
  bool execute(IObject &o) {
    static std::mt19937 rng((unsigned int)time(NULL));
    static std::uniform_int_distribution<int64_t> gen(0, 10);

    MovableObject &obj = dynamic_cast<MovableObject &>(o);

    prev = obj.getPositionRef();

    if (gen(rng) % 3 == 0) {
      obj.rotateRiht();
    } else if (gen(rng) % 4 == 0) {
      obj.rotateLeft();
    }
    if (!f) {
      obj.moveForward();

    } else {
      obj.moveBackward();
    }

    if (obj.getPositionRef() == prev) {
      f = !f;
      return false;
    }

    return false;
  }
};

const ICommand *cmd{
    new CommandBase<CommandMove>(ICommand::Priority::LONG_RUNNING)};

} // namespace CommandMove
