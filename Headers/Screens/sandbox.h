#pragma once
#include "screen.h"
#include "pixel.h"
#include "quadtree.h"
#include <vector>
#include <memory>
#include <cstddef>
#include <iostream>
#include <new>

class Sandbox : public Screen
{
public:
	Sandbox();
	~Sandbox();

	void Update() override;
	void Render() override;

  /* All Pixel instances */
  std::vector<Pixel> pixels;
private:
  Behaviour currentBehaviour = DYNAMIC;
  Mix_Music* placeSound;
  Mix_Music* deleteSound;
  float mbCooldown = 0.0f;
  float kSpaceCooldown = 0.0f;
  bool brushMenu = false;
  bool sandboxMenu = false;
  bool materialMenu = false;
  SDL_Rect worldBounds = { 0, 0, WINDOW_WIDTH/(int)RENDER_SCALE, WINDOW_HEIGHT/(int)RENDER_SCALE };
  ImVec4 color = ImVec4( 1.0f, 1.0f, 1.0f, 1.0f );
  ImVec4 backgroundColor = ImVec4( 0.0f, 0.0f, 0.0f, 1.0f );
  Color col;
  Quadtree quadtree = Quadtree(0, worldBounds);

  Color ReturnColorType(Behaviour behaviourType);
};
