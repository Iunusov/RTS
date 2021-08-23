#include "CommandMove.hpp"

#include "IMovableObject.hpp"
#include "IResponse.hpp"
#include "ObjectBase.hpp"

bool CommandMove::execute(IObject *object) noexcept {
  if (!obj) {
    obj = dynamic_cast<IMovableObject *>(object);
  }
  if (obj) {
    obj->moveTo(target);
  }

  if (object->getPosition().x <= 0 || object->getPosition().x >= 1920 - 2) {
    return true;
  }

  if (object->getPosition().y <= 0 || object->getPosition().y >= 1080 - 2) {
    return true;
  }
  return false;
}