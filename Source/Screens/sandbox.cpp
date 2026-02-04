#include "sandbox.h"
#include <algorithm>

Sandbox::Sandbox()
{
  Application& app = Application::GetInstance(); // App Singleton Reference

	std::cout << "Created Sandbox!\n";
  pixels.reserve(1700);
  placeSound = Application::GetInstance().GetSound("pixel");
  deleteSound = Application::GetInstance().GetSound("delete");
  woodSound = Application::GetInstance().GetSound("wood");
  fireSound = Application::GetInstance().GetSound("fire");
  waterSound = Application::GetInstance().GetSound("water");

  pixelTexture = SDL_CreateTexture(app.renderer,
                                    SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_STREAMING,
                                    WINDOW_WIDTH, WINDOW_HEIGHT
                                  );
}

Sandbox::~Sandbox()
{
  delete[] pixelDraw;
  SDL_DestroyTexture(pixelTexture);
}

void Sandbox::Update()
{
  Application& app = Application::GetInstance();
  mbCooldown *= 0.11f;

  brushColor.red = static_cast<int>(color.x * 255.0f);
  brushColor.green = static_cast<int>(color.y * 255.0f);
  brushColor.blue = static_cast<int>(color.z * 255.0f);
  brushColor.alpha = static_cast<int>(color.w * 255.0f);

  app.backgroundColor.red = static_cast<int>(backgroundColor.x * 255.0f);
  app.backgroundColor.green = static_cast<int>(backgroundColor.y * 255.0f);
  app.backgroundColor.blue = static_cast<int>(backgroundColor.z * 255.0f);

  /* Clear Quadtree and place Pixels on repective cells */
  quadtree.Clear();
  for (auto& pix : pixels)
    quadtree.Insert(&pix);

  /* Update every Pixel */
  for (int i = 0; i < pixels.size(); )
  {
    if (pixels[i].destroy)
    {
      pixels.erase(pixels.begin() + i);
    }
    else
    {
      std::vector<Pixel*> nearby;
      SDL_Rect area = pixels[i].position;
      area.x = std::max(worldBounds.x, area.x - 1);
      area.y = std::max(worldBounds.y, area.y - 1);
      area.w = area.w + 2;
      area.h = area.h + 2;
      quadtree.Retrieve(nearby, area);
      pixels[i].Update(nearby);

      i++;
    }
  }

  /* Check if in SDL2 Context */
  if (!ImGui::GetIO().WantCaptureMouse)
  {
    /* Create Pixels */
    if (app.mbLeft && mbCooldown < 1)
    {
    if (SDL_PointInRect(&app.mPosition, &quadtree.bounds))
    {
        int half = brushSize / 2;
        
        // Loop over brush area
        for (int dx = -half; dx <= half; dx++)
        {
            for (int dy = -half; dy <= half; dy++)
            {
                int px = app.mPosition.x + dx;
                int py = app.mPosition.y + dy;

                // Collision check with quadtree
                bool canPlace = true;
                std::vector<Pixel*> nearby;
                SDL_Rect posRect = { px, py, 1, 1 };
                quadtree.Retrieve(nearby, posRect);

                for (auto& near : nearby)
                {
                    if (near->position.x == px && near->position.y == py)
                    {
                        canPlace = false;
                        break;
                    }
                }

                if (canPlace)
                {
                    Pixel pixel(px, py, brushColor);
                    pixel.color = GetMaterialColor(brushColor, currentMaterial);
                    pixel.material = currentMaterial;

                    pixels.emplace_back(pixel);
                }
            }
        }

        switch(currentMaterial) // Play once per brush stroke
        {
        default:
          Mix_PlayMusic(placeSound, 1);
          break;
        case WOOD:
          Mix_PlayMusic(woodSound, 1);
          break;
        case FIRE:
          Mix_PlayMusic(fireSound, 1);
          break;
        case WATER:
          Mix_PlayMusic(waterSound, 1);
          break;
        }
        
        if (currentMaterial == FIRE)
          mbCooldown = 400.0f;
        else
          mbCooldown = 100.0f;
      }
    }
    else if (app.mbRight && mbCooldown < 1) /* Erase Pixels */
    {
      mbCooldown = 100.0f;
      int half = brushSize / 2;

      for (auto it = pixels.begin(); it != pixels.end(); ) 
      {
        int dx = it->position.x - app.mPosition.x;
        int dy = it->position.y - app.mPosition.y;

        // Check if pixel is inside square brush
        if (dx >= -half && dx <= half && dy >= -half && dy <= half)
        {
          it = pixels.erase(it); // erase and update iterator
        }
        else
        {
          ++it;
        }
      }

      Mix_PlayMusic(deleteSound, 1); // Play once per stroke
    }
  }

  /* Create UI Interactions */
  UserInterface();
}

void Sandbox::Render()
{
  Application& app = Application::GetInstance(); // App Singleton Reference
  std::fill_n(pixelDraw, WINDOW_WIDTH * WINDOW_HEIGHT, app.backgroundColor.GetColor());

  /* Draw brush! */
  SDL_SetRenderDrawColor(app.renderer, brushColor.red, brushColor.green, brushColor.blue, 100);
  int half = brushSize / 2; // so it grows around the center point
  for (int dx = -half; dx <= half; dx++)
  {
    for (int dy = -half; dy <= half; dy++)
    {
        if (dx*dx + dy*dy <= half*half) // inside circle
        {
            SDL_RenderDrawPoint(app.renderer, app.mPosition.x + dx, app.mPosition.y + dy);
        }
    }
  }
/*
  for(int i = 0; i < pixels.size(); i++)
  {
    Pixel& pix = pixels.at(i);
    pixelDraw[pix.position.x * pix.position.y] = pix.color.GetColor();
  }

  SDL_UpdateTexture(pixelTexture, nullptr, pixelDraw, WINDOW_WIDTH * sizeof(uint32_t));
  SDL_RenderCopy(app.renderer, pixelTexture, nullptr, nullptr);
*/
  /* Draw all "pixel" classes (materials) */
  for(int i = 0; i < pixels.size(); i++)
    pixels.at(i).Draw();
}

void Sandbox::UserInterface()
{
  Application& app = Application::GetInstance();
  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoBackground;

  ImGui::BeginMainMenuBar();
  if (ImGui::BeginMenu("File"))
  {
    if (ImGui::MenuItem("New"))
      pixels.clear();

    ImGui::MenuItem("Save");
    ImGui::Separator();

    if (ImGui::MenuItem("Exit"))
      app.done = true;

    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Edit"))
  {
    if (ImGui::MenuItem("Brush"))
      brushMenu = (brushMenu ? false : true );

    if (ImGui::MenuItem("Sandbox"))
      sandboxMenu = (sandboxMenu ? false : true);

    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Window"))
  {
    if (ImGui::MenuItem("Fullscreen"))
    {
      Uint32 flags = SDL_GetWindowFlags(app.window);

      if (flags & SDL_WINDOW_FULLSCREEN)
      {
        SDL_SetWindowFullscreen(app.window, 0);
      }
      else
      {
        SDL_SetWindowFullscreen(app.window, SDL_WINDOW_FULLSCREEN);
      }
    }
    ImGui::EndMenu();
  }

  ImGui::EndMainMenuBar();

  if (brushMenu)
  {
    ImGui::Begin("Brush!", &brushMenu, windowFlags);
    ImGui::Text("Brush options");
    ImGui::SeparatorText("Brush size");

    ImGui::SliderInt("Size", &brushSize, 1, 10);

    ImGui::SeparatorText("Brush Color");
    ImGui::ColorEdit4("Color", (float*)&color, ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_AlphaBar );

    ImGui::Text("Material list!");
    ImGui::SeparatorText("Colored Materials");

    if (ImGui::Button("Static"))
     currentMaterial = STATIC;

    ImGui::SameLine();

    if (ImGui::Button("Dynamic"))
      currentMaterial = DYNAMIC;

    ImGui::SeparatorText("Earth materials");

    if (ImGui::Button("Water"))
      currentMaterial = WATER;

    ImGui::SameLine();

    if (ImGui::Button("Milk"))
      currentMaterial = MILK;

    ImGui::SameLine();

    if (ImGui::Button("Fire"))
      currentMaterial = FIRE;

    ImGui::SameLine();

    if (ImGui::Button("Steam"))
      currentMaterial = STEAM;

    if (ImGui::Button("Dirt"))
      currentMaterial = DIRT;

    ImGui::SameLine();

    if (ImGui::Button("Stone"))
      currentMaterial = STONE;

    ImGui::SameLine();

    if (ImGui::Button("Cement"))
      currentMaterial = CEMENT;

    if (ImGui::Button("Wood"))
      currentMaterial = WOOD;

    ImGui::SameLine();

    if (ImGui::Button("Ash"))
      currentMaterial = ASH;

    ImGui::SeparatorText("Details");
    ImGui::Text("Pixel ammount: ");
    ImGui::SameLine();
    ImGui::Text(std::to_string(pixels.size()).c_str());

    ImGui::End();
  }

  if (sandboxMenu)
  {
    ImGui::Begin("Sandbox!", &sandboxMenu, windowFlags);
    ImGui::Text("Background color");
    ImGui::ColorEdit3("Color",(float*)&backgroundColor , ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_AlphaBar );
    ImGui::End();
  }
}
