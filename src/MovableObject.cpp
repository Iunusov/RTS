#include "IMovableObject.hpp"

std::mt19937 IMovableObject::rng2{(unsigned int)time(NULL)};
std::uniform_int_distribution<int64_t> IMovableObject::gen2{0, 360};
