#pragma once

template <class T, class I, class B> class Cloneable : public B {
public:
  virtual I *clone() const noexcept override final {
    return new T({(const T &)(*this)});
  }
};