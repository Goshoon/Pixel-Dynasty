#pragma once
#include "application.h"
#include "color.h"
#include "behaviour.h"
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
  
  Behaviour behaviour = DYNAMIC;
  SDL_Rect position;
  Color color;
  bool moved = true;
  bool destroy = false;

  bool CheckCollision(std::vector<Pixel*>& nearby);
  bool CheckCollision(std::vector<Pixel*>& nearby, int xoffset, int yoffset);
  bool CheckCollision(std::vector<Pixel*>& nearby, int xoffset, int yoffset, Behaviour behaviourLookup);

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
