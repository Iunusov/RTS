#include <ctime>
#include <functional>
#include <random>

#include "CommandBase.hpp"
#include "Config.hpp"
#include "IMovableObject.hpp"
#include "ObjectBase.hpp"

namespace IdleCommands {
class CommandIdle final {
  bool f = true;

public:
  bool execute(IObject &o) {
    static std::mt19937 rng((unsigned int)time(NULL));
    static std::uniform_int_distribution<int64_t> gen(0, 359);
    double diff = (double)360.0 / (double)MODEL_EXECUTE_PER_SECOND;

    IMovableObject &obj = dynamic_cast<IMovableObject &>(o);
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

public:
  bool execute(IObject &o) {
    static std::mt19937 rng((unsigned int)time(NULL));
    static std::uniform_int_distribution<int64_t> gen(0, 2);

    IMovableObject &obj = dynamic_cast<IMovableObject &>(o);
    if (gen(rng) == 1) {
      obj.rotateRiht();
    }
    if (gen(rng) == 2) {
      obj.rotateLeft();
    }

    if (!f) {
      obj.moveForward();

    } else {
      obj.moveBackward();
    }
    if (obj.getPosition().x > 30000) {
      f = !f;
      obj.setPosition(Coord{30000, obj.getPosition().y});
    } else if (obj.getPosition().x < 0) {
      f = !f;
      obj.setPosition(Coord{0, obj.getPosition().y});
    } else if (obj.getPosition().y > 30000) {
      obj.setPosition(Coord{obj.getPosition().x, 30000});
      f = !f;
    } else if (obj.getPosition().y < 0) {
      obj.setPosition(Coord{obj.getPosition().x, 0});
      f = !f;
    }
    return false;
  }
};

const ICommand *cmd{
    new CommandBase<CommandMove>(ICommand::Priority::LONG_RUNNING)};

} // namespace CommandMove
