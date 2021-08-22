#pragma once

#include <memory>

template <class T> class CommandTpl : public CommandBase {
public:
  virtual CMD clone() const noexcept {
    return std::make_unique<T>(dynamic_cast<const T &>(*this));
  }
};