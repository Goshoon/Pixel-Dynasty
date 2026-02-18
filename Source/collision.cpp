#include "collision.h"
bool Collide(SDL_Rect& rectA, SDL_Rect& rectB)
{
  return rectA.x < rectB.x + rectB.w
		&& rectA.x + rectA.w > rectB.x
		&& rectA.y < rectB.y + rectB.h
		&& rectA.y + rectA.h > rectB.y;
}