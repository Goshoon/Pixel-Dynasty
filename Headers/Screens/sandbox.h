#pragma once
#include "screen.h"
#include "pixel.h"
#include "quadtree.h"
#include "material.h"
#include "saveAndLoad.h"
#include <vector>
#include <memory>
#include <cstddef>
#include <iostream>
#include <string>
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
  void UserInterface();
  
  Material currentMaterial = DYNAMIC;
  Mix_Music* placeSound;
  Mix_Music* deleteSound;
  Mix_Music* woodSound;
  Mix_Music* fireSound;
  Mix_Music* waterSound;
  float mbCooldown = 0.0f;
  float kSpaceCooldown = 0.0f;
  bool brushMenu = false;
  bool sandboxMenu = false;
  bool materialMenu = false;
  bool showSun = false;
  int brushSize = 1;
  static const size_t MAX_PIXELS = 6500;
    
  SDL_Texture* pixelTexture;
  uint32_t* pixelDraw = new uint32_t[WINDOW_WIDTH * WINDOW_HEIGHT];
  SDL_Rect worldBounds = { 0, 0, WINDOW_WIDTH/(int)RENDER_SCALE, WINDOW_HEIGHT/(int)RENDER_SCALE };
  ImVec4 color = ImVec4( 1.0f, 1.0f, 1.0f, 1.0f );            // ImGui Color wheel data (has to be casted into RGB)
  ImVec4 backgroundColor = ImVec4( 0.0f, 0.0f, 0.0f, 1.0f );  // ImGui Color wheel data (has to be casted into RGB)
  Color brushColor;                                           // ImGui to RGB Format
  Quadtree quadtree = Quadtree(0, worldBounds);               // World Quadtree, relative to screen and hardcoded, lol
};
