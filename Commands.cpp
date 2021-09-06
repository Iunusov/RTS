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
  Coord prev{};

public:
  bool execute(IObject &o) {
    static std::mt19937 rng((unsigned int)time(NULL));
    static std::uniform_int_distribution<int64_t> gen(0, 10);

    IMovableObject &obj = dynamic_cast<IMovableObject &>(o);

    prev = obj.getPosition();

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
    if (obj.getPosition().x > MAX_COORD) {
      f = !f;
      obj.setPosition(Coord{MAX_COORD, obj.getPosition().y});
    } else if (obj.getPosition().x < 0 + 200) {
      f = !f;
      obj.setPosition(Coord{200, obj.getPosition().y});
    } else if (obj.getPosition().y > MAX_COORD) {
      obj.setPosition(Coord{obj.getPosition().x, MAX_COORD});
      f = !f;
    } else if (obj.getPosition().y < 0 + 200) {
      obj.setPosition(Coord{obj.getPosition().x, 200});
      f = !f;
    }

    if (obj.getPosition() == prev) {
      f = !f;
      return false;
    }

    return false;
  }
};

const ICommand *cmd{
    new CommandBase<CommandMove>(ICommand::Priority::LONG_RUNNING)};

} // namespace CommandMove
