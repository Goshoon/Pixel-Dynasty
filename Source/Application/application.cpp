#include "application.h"

/* Asset Management */
void Application::AddTexture(const std::string& ID, const char* fileDir)
{
	SDL_Texture* texture = IMG_LoadTexture(renderer, fileDir);

  if (!texture)
  {
    std::cout << "Error! couldn't load image: " << ID << "\n";
  }
  else 
  {
	  images[ID] = texture;
  }
}

SDL_Texture* Application::GetTexture(const std::string& ID) 
{
	auto it = images.find(ID);
	return it != images.end() ? it->second : nullptr;
}

void Application::AddSound(const std::string& ID, const char* fileDir)
{
  Mix_Music* sound = Mix_LoadMUS(fileDir);
  if (!sound)
  {
    std::cout << "Error! couldnt load sound: " << ID << "\n";
  }
  else
  {
    sounds[ID] = sound;
  }
}

Mix_Music* Application::GetSound(const std::string& ID)
{
  auto it = sounds.find(ID);
  return it != sounds.end() ? it->second : nullptr;
}

/* Constructors */
Application::Application()
{
	window = SDL_CreateWindow(
		WINDOW_NAME, 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN 
	);

	renderer = SDL_CreateRenderer(
		window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	done = false;
	mbLeft = false;
	mbRight = false;

  SDL_RenderSetScale(renderer, RENDER_SCALE, RENDER_SCALE);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  SDL_Surface* icon = IMG_Load("icon.png");
  SDL_SetWindowIcon(window, icon);
  SDL_FreeSurface(icon);

  backgroundColor.red = 0;
  backgroundColor.green = 0;
  backgroundColor.blue = 0;
  backgroundColor.alpha = 255;
}

Application::~Application()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	for (auto& pair : images)
	{
		SDL_DestroyTexture(pair.second);
	}
  for(auto& pair : sounds)
  {
    Mix_FreeMusic(pair.second);
  }
}

/* Rendering */
void Application::Display()
{
	SDL_RenderClear( renderer );	
}

void Application::DrawEverything()
{
  SDL_RenderSetScale(renderer, 1.0f, 1.0f);
	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
	SDL_SetRenderDrawColor( renderer, backgroundColor.red, backgroundColor.green, backgroundColor.blue, 255 );
  SDL_RenderSetScale(renderer, RENDER_SCALE, RENDER_SCALE);
	SDL_RenderPresent(renderer);
}

void Application::Input() /* Teclas e interacciones con la ventana */
{
	SDL_Event event;
	while ( SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		switch (event.type)
		{
			case SDL_QUIT:
				done = true;
			break;
      case SDL_MOUSEMOTION:
        SDL_GetMouseState(&mPosition.x, &mPosition.y);
        mPosition.x /= (int)RENDER_SCALE;
        mPosition.y /= (int)RENDER_SCALE;
      break;
			case SDL_KEYDOWN:
				InputPressed(&event);
			break;
			case SDL_KEYUP:
				InputReleased(&event);
			break;
			case SDL_MOUSEBUTTONDOWN:
				switch(event.button.button)
				{
				case SDL_BUTTON_LEFT:
					mbLeft = true;
				break;
				case SDL_BUTTON_RIGHT:
					mbRight = true;
				break;
				}
			break;
			case SDL_MOUSEBUTTONUP:
				mbLeft = false;
				mbRight = false;
			break;
        }
    }
}

void Application::InputReleased(SDL_Event* event)
{
	switch (event->key.keysym.sym)
	{
		case SDLK_F1:
		{
			hideInterface = true;
			break;
		}
		case SDLK_ESCAPE:
		{
			done = true;
		}
  	break;
    case SDLK_SPACE:
		{
      kSpace = false;
    }
		break;
	}
}

void Application::InputPressed(SDL_Event* event)
{
	switch (event->key.keysym.sym)
	{
		case SDLK_F1:
		{
			hideInterface = false;
			break;
		}
    case SDLK_SPACE:
    {
      kSpace = true;
      break;
    }
	}
}

void Application::DrawRectangle(int x, int y, int width, int height, SDL_Color color)
{
    // Establecer el color
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Crear area donde se va a dibujar
    SDL_Rect squareRect = { x, y, width, height }; // { x, y, ancho, alto }

    // Dibujar el rectangulo en el area
    SDL_RenderFillRect(renderer, &squareRect);
}

void Application::RenderImage( SDL_Texture* image, int x, int y )
{
	int width, height;
	SDL_QueryTexture(image, NULL, NULL, &width, &height);
	SDL_Rect dst = { x, y, width, height };
	SDL_RenderCopy(renderer, image, NULL, &dst);
}

void Application::RenderImage( SDL_Texture* image, int x, int y, int w, int h )
{
	SDL_Rect dst = { x, y, w, h };
	SDL_RenderCopy(renderer, image, NULL, &dst);
}
