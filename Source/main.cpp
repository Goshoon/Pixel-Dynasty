#include "main.h"
using std::cout, std::cin;
int main(int argc, char* argv[])
{
	// Initialize SDL2 & ImGui
	Inititialize();

	// Create initial scene
  std::unique_ptr<Screen> currentScreen = std::make_unique<Sandbox>();
  Application& app = Application::GetInstance();

	while(!app.done)
	{
		app.Input(); /* User inputs */

		ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    currentScreen->Update(); /* Scene update position and input */
		app.Display(); /* Mostrar interfaces y Actualizar pantalla */
		currentScreen->Render(); /* Render elements on the scene */
		app.DrawEverything(); /* Refresh window and clear renderer */
	}

	// Cerrar dependencias y limpiar memoria
	Quit();
	
	return 0;
}
