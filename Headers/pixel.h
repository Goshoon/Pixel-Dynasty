#pragma once
#include "application.h"
#include "color.h"
#include "material.h"
#include "collision.h"
#include "gmath.h"

#include "imgui.h"

#include <cstdint>
#include <vector>
#include <memory>
#include <algorithm>
#include <random>

class Pixel
{
public:
  Pixel();
  Pixel(int x, int y, Color col);
  
  Material material = DYNAMIC;
  SDL_Rect position;
  Color color;
  bool moved = true;
  bool destroy = false;

  void save(std::ostream& out) const
  {
        out << static_cast<int>(material) << " "
            << position.x << " "
            << position.y << " "
            << position.w << " "
            << position.h << " "
            << color.red << " "
            << color.green << " "
            << color.blue << " "
            << color.alpha << " "
            << moved << "\n";
  }

  void load(std::istream& in)
  {
    int mat;
    in >> mat;
    material = static_cast<Material>(mat);

    in >> position.x
      >> position.y
      >> position.w
      >> position.h
      >> color.red
      >> color.green
      >> color.blue
      >> color.alpha
      >> moved;
  }

  bool CheckCollision(std::vector<Pixel*>& nearby);
  bool CheckCollision(std::vector<Pixel*>& nearby, int xoffset, int yoffset);
  bool CheckCollision(std::vector<Pixel*>& nearby, int xoffset, int yoffset, Material materialLookup);

  void Update(std::vector<Pixel*>& nearby);
  void Draw();
private:
  short lifetime;
  SDL_Point lastPosition;
  SDL_Point worldBorder;
  void Gravity(std::vector<Pixel*>& nearby);
  void Unstuck(std::vector<Pixel*>& nearby, int limit);
  void GasBehaviour(std::vector<Pixel*>& nearby);
};
