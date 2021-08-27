#include "CommandBase.hpp"
#include "IMovableObject.hpp"
#include "ObjectBase.hpp"

#include <ctime>
#include <random>

const ICommand *command_idle{new CommandBase{
    ICommand::Priority::IDLE, [](size_t objectId) {
      auto obj = dynamic_cast<IMovableObject *>(ObjectBase::getById(objectId));
      static std::mt19937 rng((unsigned int)time(NULL));
      static std::uniform_int_distribution<int64_t> gen(0, 359);
      if (gen(rng) == 50) {
        return false;
      }
      obj->fire_angle = gen(rng);
      return false;
    }}};

const ICommand *command_move{new CommandBase{
    ICommand::Priority::LONG_RUNNING, [](size_t objectId) {
      auto obj = dynamic_cast<IMovableObject *>(ObjectBase::getById(objectId));
      if (obj) {
        obj->moveTo(Coord{400, 300, 0});
        return false;
      }
      return true;
    }}};
