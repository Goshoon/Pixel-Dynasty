#pragma once
#include "screen.h"
#include "pixel.h"
#include "quadtree.h"
#include "behaviour.h"
#include <vector>
#include <memory>
#include <cstddef>
#include <iostream>
#include <new>

class Sandbox : public Screen
{
public:
	Sandbox();

	void Update() override;
	void Render() override;

  /* All Pixel instances */
  std::vector<Pixel> pixels;
private:
  void UserInterface();

  Behaviour currentBehaviour = DYNAMIC;
  Mix_Music* placeSound;
  Mix_Music* deleteSound;
  float mbCooldown = 0.0f;
  float kSpaceCooldown = 0.0f;
  bool brushMenu = false;
  bool sandboxMenu = false;
  bool materialMenu = false;
  bool showSun = false;
  
  SDL_Rect worldBounds = { 0, 0, WINDOW_WIDTH/(int)RENDER_SCALE, WINDOW_HEIGHT/(int)RENDER_SCALE };
  ImVec4 color = ImVec4( 1.0f, 1.0f, 1.0f, 1.0f );            // ImGui Color wheel data (has to be casted into RGB)
  ImVec4 backgroundColor = ImVec4( 0.0f, 0.0f, 0.0f, 1.0f );  // ImGui Color wheel data (has to be casted into RGB)
  Color brushColor; // ImGui to RGB Format
  Quadtree quadtree = Quadtree(0, worldBounds); // World Quadtree, relative to screen and hardcoded, lol
};
