#include "context.h"
#include <time.h>

void LoadSounds()
{
  Application& app = Application::GetInstance();
  app.AddSound("pixel", "Resources/Sound/pixel.ogg");
  app.AddSound("delete", "Resources/Sound/delete.ogg");
  app.AddSound("wood", "Resources/Sound/wood.ogg");
  app.AddSound("fire", "Resources/Sound/fire.ogg");
  app.AddSound("water", "Resources/Sound/water.ogg");
}

void Inititialize() // Iniciar SDL y SDL_Image
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG;
  if (SDL_Init(SDL_INIT_AUDIO) < 0) 
    std::cerr << "SDL_Init Error: " << SDL_GetError() << '\n';

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) 
    std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << '\n';

	// Inicializar Dear ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui::StyleColorsDark(); // Tema

  // Setup Platform/Renderer backends
  Application& app = Application::GetInstance();
  ImGui_ImplSDL2_InitForSDLRenderer(app.window, app.renderer);
  ImGui_ImplSDLRenderer2_Init(app.renderer);

  LoadSounds();
}
void Quit() // Cerrar aplicacion
{
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  IMG_Quit();
  Mix_CloseAudio();
	SDL_Quit();
}
