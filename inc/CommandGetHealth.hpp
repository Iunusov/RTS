#pragma once

#include "CommandBase.hpp"

#include "CommandBase.hpp"
#include "CommandTpl.hpp"
#include "IResponse.hpp"
#include "ObjectBase.hpp"

class CommandGetHealth final : public CommandTpl<CommandGetHealth> {
public:
  CommandGetHealth() noexcept { priority = ICommand::Priority::ONE_CYCLE; }
  virtual IResponse execute(IObject *object) noexcept override {
    auto obj = dynamic_cast<ObjectBase *>(object);
    if (obj) {
      return {obj->getHealth()};
    }
    return IResponse{int64_t{}};
  }
};
