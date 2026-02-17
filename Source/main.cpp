#include "main.h"
using std::cout, std::cin;
int main(int argc, char* argv[])
{
	/* Initialize SDL2 & ImGui */
	Inititialize();

	/* Create initial scene */
  	std::unique_ptr<Screen> currentScreen = std::make_unique<MainMenu>();
  	Application& app = Application::GetInstance();

	const int TARGET_FPS = 60;
	const int FRAME_TIME_MS = 1000 / TARGET_FPS;
	uint32_t frameStart = 0;
	uint32_t frameTime = 0;

	while(!app.done)
	{
		frameStart = SDL_GetTicks();

		app.Input(); // User inputs

		/* Handle scene swapping */
		if (currentScreen->nextScene)
		{
			if (dynamic_cast<MainMenu*>(currentScreen.get()))
			{
				currentScreen = std::make_unique<Sandbox>();
			}
			else if (dynamic_cast<Sandbox*>(currentScreen.get()))
			{
				currentScreen = std::make_unique<MainMenu>();
			}

			/* Reset nextScene on the new/current screen if present */
			currentScreen->nextScene = false;
		}

		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		currentScreen->Update(); 		// Scene update position and input
		app.Display(); 					// Show interfaces and update frame
		currentScreen->Render(); 		// Render elements on the scene
		app.DrawEverything(); 			// Refresh window and clear renderer

		// Frame rate limiting
		frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < FRAME_TIME_MS)
		{
			SDL_Delay(FRAME_TIME_MS - frameTime);
		}
	}

	Quit(); // Close all dependencies and free memory
	return 0;
}
