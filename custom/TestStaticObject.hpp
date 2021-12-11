#pragma once

#include "IObject.hpp"
#include "IStaticObject.hpp"

class TestStaticObject final
    : public Cloneable<TestStaticObject, IObject, IStaticObject> {
private:
  const std::vector<Coord> points{
      {453, 50},   {424, 100}, {395, 150}, {367, 200}, {338, 250},
      {309, 300},  {280, 350}, {250, 400}, {219, 450}, {189, 500},
      {159, 550},  {129, 600}, {98, 650},  {67, 700},  {37, 750},
      {7, 800},    {56, 850},  {128, 900}, {213, 950}, {312, 1000},
      {509, 1050}, {580, 50},  {675, 100}, {746, 150}, {818, 200},
      {822, 250},  {868, 300}, {940, 350}, {930, 400}, {900, 450},
      {869, 500},  {839, 550}, {808, 600}, {777, 650}, {746, 700},
      {717, 750},  {688, 800}, {659, 850}, {629, 900}, {600, 950},
      {571, 1000}, {542, 1050}};

public:
  const std::vector<Coord> &getPoints() CNOF { return points; }
  virtual double getWidth() const noexcept override { return 957.0; }
  virtual double getHeight() const noexcept override { return 1065.0; }
};
