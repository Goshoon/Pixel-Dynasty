#include "material.h"

Color GetMaterialColor(Color& brushColor, Material material)
{
  switch(material)
  {
    default: return brushColor;
        case WATER:  return Color{0, 0, 155, 225};
        case MILK:   return Color{215, 215, 215, 225};
        case FIRE:   return Color{155, 0, 0, 200};
        case DIRT:   return Color{67, 40, 24, 255};
        case STONE:  return Color{50, 50, 50, 255};
        case CEMENT: return Color{25, 25, 25, 255};
        case WOOD:   return Color{111, 78, 55, 255};
        case STEAM:  return Color{155, 155, 155, 125};
        case ASH:    return Color{100, 100, 100, 255};
  }
}