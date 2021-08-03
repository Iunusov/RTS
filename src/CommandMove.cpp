#include "CommandMove.hpp"

#include "IMovableObject.hpp"
#include "IResponse.hpp"
#include "ObjectBase.hpp"

IResponse CommandMove::execute(IObject *object) noexcept {
  if (!obj) {
    obj = dynamic_cast<IMovableObject *>(object);
  }
  if (obj) {
    obj->moveTo(target);
  }
  return IResponse{"finished"};
}