#pragma once
#include <cstdint>

#ifndef COLOR_H
#define COLOR_H

struct Color
{
  int red, green, blue, alpha;
  uint32_t GetColor()
  {
    uint8_t r = red;
    uint8_t g = green;
    uint8_t b = blue;
    uint8_t a = blue;
    return ( a << 24) | (r << 16) | (g << 8) | b;
  }
};
#endif
