#pragma once
#include "color.h"

#ifndef PIXEL_H
#define PIXEL_H

enum Material 
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
  STEAM,
  ASH,
  DYNAMITE
};

Color GetMaterialColor(Color& brushColor, Material behaviourType);

#endif