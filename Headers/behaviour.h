#pragma once
#include "color.h"

#ifndef PIXEL_H
#define PIXEL_H

enum Behaviour 
{
  STATIC,
  DYNAMIC,
  WATER,
  MILK,
  FIRE,
  DIRT,
  STONE,
  CEMENT,
  WOOD,
  STEAM
};

Color GetMaterialColor(Color& brushColor, Behaviour behaviourType);

#endif