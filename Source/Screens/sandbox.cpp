#include "sandbox.h"
Sandbox::Sandbox()
{
	std::cout << "Created Sandbox!\n";
  pixels.reserve(1700);
  placeSound = Application::GetInstance().GetSound("pixel");
  deleteSound = Application::GetInstance().GetSound("delete");
}

void Sandbox::Update()
{
  Application& app = Application::GetInstance();
  mbCooldown *= 0.1f;

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
      quadtree.Retrieve(nearby, pixels[i].position);
      pixels[i].Update(nearby);

      i++;
    }
  }

  /* Check if in SDL2 Context */
  if (!ImGui::GetIO().WantCaptureMouse)
  {
    /* Create Pixels */
    if(app.mbLeft && mbCooldown < 1)
    {
      if (SDL_PointInRect(&app.mPosition, &quadtree.bounds))
      {
        std::cout << pixels.size() << "\n";
        bool canPlace = true;
        std::vector<Pixel*> nearby;
        SDL_Rect mPosition = { app.mPosition.x, app.mPosition.y, 1, 1 };
        quadtree.Retrieve(nearby, mPosition);

        for(auto& near : nearby)
        {
          if (near->position.x == app.mPosition.x && near->position.y == app.mPosition.y)
          {
            canPlace = false;
            break;
          }
        }

        if (canPlace)
        {
          Mix_PlayMusic(placeSound, 1);
          Pixel pixel = Pixel(app.mPosition.x, app.mPosition.y, brushColor);
          pixel.color = GetMaterialColor(brushColor, currentBehaviour);
          pixel.behaviour = currentBehaviour;

          pixels.emplace_back(pixel);
          mbCooldown = 100.0f;
        }
      }
    }
    else if (app.mbRight && mbCooldown < 1) /* Erase Pixels */
    {
      mbCooldown = 100.0f;
      for (auto it = pixels.begin(); it != pixels.end(); ) 
      {
        if (SDL_PointInRect(&app.mPosition, &it->position) == SDL_TRUE) 
        {
          // Erase
          pixels.erase(it);
          Mix_PlayMusic(deleteSound, 1);
        }
        else
        {
          ++it;
        }
      }
    } 
  }

  /* Create UI Interactions */
  UserInterface();
}

void Sandbox::Render()
{
  Application& app = Application::GetInstance(); // App Singleton Reference

  SDL_Color sunColor = { 255, 255, 0, 100 };
  SDL_SetRenderDrawColor(app.renderer, brushColor.red, brushColor.green, brushColor.blue, 100);
  SDL_RenderDrawPoint(app.renderer, app.mPosition.x, app.mPosition.y);
  for(int i = 0; i < pixels.size(); i++)
    pixels.at(i).Draw();
}

void Sandbox::UserInterface()
{
  Application& app = Application::GetInstance();

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
    ImGui::Begin("Brush!", &brushMenu);
    ImGui::Text("Brush options");
    ImGui::SeparatorText("Brush size");

    ImGui::SeparatorText("Brush Color");
    ImGui::ColorEdit4("Color", (float*)&color, ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_AlphaBar );

    ImGui::Text("Material list!");
    ImGui::SeparatorText("Colored Materials");

    if (ImGui::Button("Static"))
     currentBehaviour = STATIC;

    ImGui::SameLine();

    if (ImGui::Button("Dynamic"))
      currentBehaviour = DYNAMIC;

    ImGui::SeparatorText("Earth materials");

    if (ImGui::Button("Water"))
      currentBehaviour = WATER;

    ImGui::SameLine();

    if (ImGui::Button("Milk"))
      currentBehaviour = MILK;

    ImGui::SameLine();

    if (ImGui::Button("Fire"))
      currentBehaviour = FIRE;

    ImGui::SameLine();

    if (ImGui::Button("Dirt"))
      currentBehaviour = DIRT;

    ImGui::SameLine();

    if (ImGui::Button("Stone"))
      currentBehaviour = STONE;

    ImGui::SameLine();

    if (ImGui::Button("Cement"))
      currentBehaviour = CEMENT;

    if (ImGui::Button("Wood"))
      currentBehaviour = WOOD;

    ImGui::SameLine();

    if (ImGui::Button("Steam"))
      currentBehaviour = STEAM;

    ImGui::End();
  }

  if (sandboxMenu)
  {
    ImGui::Begin("Sandbox!", &sandboxMenu);
    ImGui::Text("Background color");
    ImGui::ColorEdit3("Color",(float*)&backgroundColor , ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_AlphaBar );
    ImGui::Checkbox("Sun", &showSun);
    ImGui::End();
  }
}
