#include "Config.hpp"
#include "Math.hpp"
#include "TestObject.hpp"
#include "TestStaticObject.hpp"

#include <cstddef>
#include <iostream>
#include <random>
#include <vector>

static std::mt19937 rng((unsigned int)time(NULL));
static std::uniform_int_distribution<int64_t> gen(0, MAX_COORD);

namespace CommandMove {
extern const ICommand *cmd;
}

void addTestData(std::vector<IObject *> &Objects) {
  std::cout << "creating test map, please wait..." << std::endl;

  for (int i(0); i < MAX_COORD / 5; ++i) {
    TestStaticObject *staticobject = new TestStaticObject{};
    staticobject->setPosition(
        Coord{(decltype(Coord::x))gen(rng), (decltype(Coord::y))gen(rng)});
    staticobject->teleportTo(staticobject->getPosition());

    if (staticobject->getPosition().x < 500 ||
        staticobject->getPosition().y < 500)
      continue;
    if (staticobject->getPosition().x >= MAX_COORD - 500 ||
        staticobject->getPosition().y >= MAX_COORD - 500)
      continue;

    Collisions::getInstance()->update_static(*staticobject);
    Objects.emplace_back(staticobject);
  }

  for (size_t i(0); i < MAX_COORD * 10; i++) {
    IObject *obj = new TestObject();
    obj->setPosition(
        Coord{(decltype(Coord::x))gen(rng), (decltype(Coord::y))gen(rng)});

    // for (auto &o : Objects) {
    // if (o->getPosition().distance(obj->getPosition()) < 200) {

    // break;
    //}
    //  }

    while (

        Collisions::getInstance()->checkCollisions(*((IMovableObject *)obj))) {
      static size_t c{};
      if (++c > 100000) {
        std::cout << std::endl
                  << "created " << Objects.size() << " units" << std::endl
                  << std::endl;
        return;
      }
      obj->setPosition(
          Coord{(decltype(Coord::x))gen(rng), (decltype(Coord::y))gen(rng)});
    }

    obj->setHeading((double)(gen(rng) % 360));
    obj->teleportTo(obj->getPosition());

    Collisions::getInstance()->update(*(IMovableObject *)obj);
    Objects.emplace_back(obj);
    if (obj->getId() % 2)
      obj->acceptCommand(*CommandMove::cmd);
  }

  std::cout << std::endl
            << "created " << Objects.size() << " units" << std::endl
            << std::endl;
}
