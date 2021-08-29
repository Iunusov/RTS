#include "TestObject.hpp"

#include <ctime>
#include <random>

#include "VideoContext.hpp"

namespace IdleCommands {
extern const ICommand *cmd;
}

TestObject::TestObject(const Coord &pos) noexcept {
  position = pos;
  acceptCommand(*IdleCommands::cmd);
}
