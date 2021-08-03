#pragma once

#include "CommandBase.hpp"
#include "CommandTpl.hpp"
#include "IMovableObject.hpp"
#include "IObject.hpp"
#include "IResponse.hpp"
#include "ObjectBase.hpp"

class ObjectBase;

class CommandMove final : public CommandTpl<CommandMove> {
private:
  Coord target;
  IMovableObject *obj = nullptr;

public:
  CommandMove(const Coord &position) noexcept : target{position} {}
  virtual IResponse execute(IObject *object) noexcept override;
};
