#pragma once

#include "CppHacks.hpp"

template <class T, class I, class B> class Cloneable : public B {
public:
  I *clone() CNOF { return new T({(const T &)(*this)}); }
};