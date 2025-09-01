// https://www.youtube.com/watch?v=iUq0waTh9Pw&ab_channel=%E2%98%86111loggedin%2F%2Fxxenaa%E2%99%A1
#include "pixel.h"

Pixel::Pixel()
{}

Pixel::Pixel(int x, int y, Color col)
{
  lifetime = 0;
  position.x = x;
  position.y = y;
  position.w = 1;
  position.h = 1;
  color = col;
  worldBorder.x = WINDOW_WIDTH / RENDER_SCALE - 1;
  worldBorder.y = WINDOW_HEIGHT / RENDER_SCALE - 1;
}

void Pixel::Update(std::vector<Pixel*>& nearby)
{
  /* Particle behaviours */
  switch(behaviour)
  {
    default:
      break;

    case STONE:
      Gravity(nearby);
      break;

    case FIRE:
      GasBehaviour(nearby);

      if (lifetime > 1000)
        color.alpha--;

      if (color.alpha < 140)
        destroy = true;
    break;
    case STEAM:
      GasBehaviour(nearby);

      if (lifetime > 1200)
        color.alpha--;

      if (color.alpha < 100)
        destroy = true;
    break;

    case DYNAMIC:
      Gravity(nearby);
      break;

    case MILK:
    case WATER:
      {
        /* Movement and reactions to enviromment */
        Gravity(nearby);
        bool leftCol = CheckCollision(nearby, -1, 0);
        bool rightCol = CheckCollision(nearby, 1, 0);
        bool leftBellowCol = CheckCollision(nearby, -1, 1);
        bool rightBellowCol = CheckCollision(nearby, 1, 1);

        if (CheckCollision(nearby, 0, 1))
        {
          if(!leftCol && !rightCol)
          {
            std::random_device rd;
            std::mt19937 gen(rd());

            std::uniform_int_distribution<> dist(-1, 1);

            int random_number = dist(gen);

            if(random_number == 0 && position.x > 0)
            {
              position.x--;
            }
            else if (position.x < worldBorder.x)
            {
              position.x++;
            }
          }
          else if (!leftBellowCol && position.x < 0 && position.y > worldBorder.y)
          {
            position.x--;
            position.y++;
          }
          else if (!rightBellowCol && position.x > worldBorder.x && position.y > worldBorder.y)
          {
            position.x++;
            position.y++;
          }
          else if (!leftCol && position.x > 0)
          {
            position.x--;
          }
          else if (!rightCol && position.x < worldBorder.x)
          {
            position.x++;
          }
        }

        /* Other material reactions */
        if (CheckCollision(nearby, 0, -1, FIRE))
        {
          behaviour = STEAM;
          Color tmp;
          color = GetMaterialColor(tmp, STEAM);
        }
        if (CheckCollision(nearby, 0, 1, FIRE))
        {
          behaviour = STEAM;
          Color tmp;
          color = GetMaterialColor(tmp, STEAM);
        }

        /* Harsh unstuck */
        if(CheckCollision(nearby, 0, 0))
        {
          Unstuck(nearby, 5);
        }

        /* Change alpha quickly */
        std::random_device rd;  
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(100, 255);
        int random_number = dist(gen);
        color.alpha = random_number;
      }
      break;

    case DIRT:
      Gravity(nearby);
      bool topCol = CheckCollision(nearby, 0, -1);

      if (!topCol)
      {
        color.red = 0;
        color.green = 40;
        color.blue = 0;
        color.alpha = 255;
      }
      else
      {
        color.red = 67;
        color.green = 40;
        color.blue = 24;
        color.alpha = 255;
      }
      break;
  }

  /* Stay within world limits */
  position.x = Clamp(position.x, 0, worldBorder.x);
  position.y = Clamp(position.y, 0, worldBorder.y);
}

void Pixel::Draw()
{
  Application& app = Application::GetInstance();
  SDL_SetRenderDrawColor(app.renderer, color.red, color.green, color.blue, color.alpha);
  SDL_RenderDrawRect(app.renderer, &position);
}

/* Gravity */
void Pixel::Gravity(std::vector<Pixel*>& nearby)
{
  if (position.y < WINDOW_HEIGHT / RENDER_SCALE - 1)
  {
    bool canGoDown = true;

    if (CheckCollision(nearby, 0, 1))
      canGoDown = false;

    if (canGoDown)
    {
      position.y++;
    }
  }
}

/* Collision checks */
bool Pixel::CheckCollision(std::vector<Pixel*>& nearby)
{
  for(auto& near : nearby)
  {
    if (this == near) continue;

    if (Collide(position, near->position))
      return true;
  }

  return false;
}

bool Pixel::CheckCollision(std::vector<Pixel*>& nearby, int xoffset, int yoffset)
{
  SDL_Rect pos = position;
  pos.x += xoffset;
  pos.y += yoffset;

  for(auto& near : nearby)
  {
    if (this == near) continue;

    if (Collide(pos, near->position))
    {
      return true;
    }
  }

  return false;
}

bool Pixel::CheckCollision(std::vector<Pixel*>& nearby, int xoffset, int yoffset, Behaviour behaviourLookup)
{
  SDL_Rect pos = position;
  pos.x += xoffset;
  pos.y += yoffset;

  for (auto& near : nearby)
  {
    if (this == near) continue;

    if (Collide(pos, near->position) && near->behaviour == behaviourLookup)
      return true;
  }

  return false;
}

void Pixel::Unstuck(std::vector<Pixel*>& nearby, int limit)
{
  if(CheckCollision(nearby, 0, 0))
  {
    for(int i = 0; i < limit; i++)
    {
      // Right
      if(!CheckCollision(nearby, i, 0))
      {
        position.x+=i;
        break;
      }
      // Left
      if(!CheckCollision(nearby, -i, 0))
      {
        position.x-=i;
        break;
      }
        
      // Up
      if(!CheckCollision(nearby, 0, -i))
      {
        position.y-=i;
        break;
      }
      // Down
      if(!CheckCollision(nearby, 0, i))
      {
        position.y+=i;
        break;
      }
      
      //Top Right
      if(!CheckCollision(nearby, i, -i))
      {
        position.x+=i;
        position.y-=i;
        break;
      }
      //Top Left
      if(!CheckCollision(nearby, -i, -i))
      {
        position.x-=i;
        position.y-=i;
        break;
      }
        
      //Bottom Right
      if(!CheckCollision(nearby, i, i))
      {
        position.x+=i;
        position.y+=i;
        break;
      }
        
      //Bottom Left
      if(!CheckCollision(nearby, -i, -i))
      {
        position.x-=i;
        position.y+=i;
        break;
      }
    }
  }
}

void Pixel::GasBehaviour(std::vector<Pixel*>& nearby)
{
  lifetime++;
  if (!CheckCollision(nearby, 0, -1))
  {
    position.y--;
  }
  else
  {
    bool leftCol = CheckCollision(nearby, -1, 0);
    bool rightCol = CheckCollision(nearby, 1, 0);

    if (!leftCol && !rightCol)
    {
      std::random_device rd;
      std::mt19937 gen(rd());

      std::uniform_int_distribution<> dist(-1, 1);

      int random_number = dist(gen);

      if(random_number == 0 && position.x > 0)
      {
        position.x--;
      }
      else if (position.x < worldBorder.x)
      {
        position.x++;
      }
    }
    else if (!leftCol && position.x > 0)
    {
      position.x--;
    }
    else if (!rightCol && position.x < worldBorder.x)
    {
      position.x++;
    }
    else
    {
      bool leftBellowCol = CheckCollision(nearby, -1, 1);
      bool rightBellowCol = CheckCollision(nearby, 1, 1);
      if (leftBellowCol)
      {
        position.x--;
        position.y++;
      }
      else if (rightBellowCol)
      {
        position.x++;
        position.y++;
      }
    }
  }
}