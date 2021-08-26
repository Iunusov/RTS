#pragma once

#include "CommandBase.hpp"
#include "CommandTpl.hpp"
#include "IResponse.hpp"
#include "ObjectBase.hpp"

#include <ctime>
#include <random>

class CommandIdle final : public CommandTpl<CommandIdle> {
private:
  size_t k{0};

public:
  CommandIdle() noexcept { priority = ICommand::Priority::IDLE; }
  virtual bool execute(IObject *object) noexcept override {
    static std::mt19937 rng((unsigned int)time(NULL));
    static std::uniform_int_distribution<int64_t> gen(0, 359);

    if (k++ % 20 != 0) {
      return false;
    }

    if (gen(rng) == 50) {
      return false;
    }

    auto obj = dynamic_cast<ObjectBase *>(object);
    if (obj) {
      obj->fire_angle = gen(rng);
    }
    return false;
  }
};
