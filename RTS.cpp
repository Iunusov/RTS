// RTS.cpp : Defines the entry point for the application.
//

#include "RTS.h"
#include "CommandGetHealth.hpp"
#include "CommandMove.hpp"
#include "IResponse.hpp"
#include "TestObject.hpp"
#include <thread>

std::vector<TestObject> OBJECTS{1000};

int main() {
  TestObject obj;
  CommandGetHealth cmd;
  CommandMove mv{Coord{777, 777, 777}};

  IResponse res = OBJECTS[OBJECTS.size() - 1].acceptCommand(cmd);

  std::cout << res.get_int << std::endl;
  for (auto &v : OBJECTS) {
    v.acceptCommand(mv);
  }

  while (true) {
    for (auto &v : OBJECTS) {
      v.execute();
    }
    std::cout << "done cycle" << std::endl;
  }

  return 0;
}
