// RTS.cpp : Defines the entry point for the application.
//

#include "RTS.h"
#include "CommandGetHealth.hpp"
#include "CommandMove.hpp"
#include "IResponse.hpp"
#include "TestObject.hpp"
#include <thread>
#include <unordered_map>
#include <memory>

std::unordered_map<size_t, std::unique_ptr<IObject>> OBJCTS;
constexpr const size_t OBJCTS_COUNT{1000000};

int main() {

OBJCTS.reserve(OBJCTS_COUNT);

for(size_t i(0); i<OBJCTS_COUNT; ++i){
	auto &&obj = std::make_unique<TestObject>();
	OBJCTS[obj->getId()] = std::move(obj);
}
  std::cout<<"done"<<std::endl;
  //system ("pause");
  return 0;
}
