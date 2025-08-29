// https://www.youtube.com/watch?v=iUq0waTh9Pw&ab_channel=%E2%98%86111loggedin%2F%2Fxxenaa%E2%99%A1
#include "pixel.h"

int Clamp(int variable, int minimum, int maximum)
{
  if (variable < minimum)
    return minimum;

  if (variable > maximum)
    return maximum;

  return variable;
}

Pixel::Pixel()
{}

Pixel::Pixel(int x, int y, Color col)
{
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
    case DYNAMIC:
      Gravity(nearby);
      break;
    case WATER:
      {
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

            std::uniform_int_distribution<> dist(0, 1);

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
      }
      break;

    case DIRT:
      Gravity(nearby);
      break;
    case STONE:
      Gravity(nearby);
      break;
    case FIRE:
      bool leftCol = CheckCollision(nearby, -1, 0);
      bool rightCol = CheckCollision(nearby, 1, 0);
      bool leftTopCol = CheckCollision(nearby, -1, -1);
      bool rightTopwCol = CheckCollision(nearby, 1, -1);

      /*  */
      if(!CheckCollision(nearby, 0, -1))
        position.y--;

      /*  */
      if(!leftCol && !rightCol)
      {
        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_int_distribution<> dist(0, 1);

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
      else if (!leftTopCol && position.x < 0 && position.y > worldBorder.y)
      {
        position.x--;
        position.y++;
      }
      else if (!rightTopwCol && position.x > worldBorder.x && position.y > worldBorder.y)
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
      break;
  }

  /* Harsh unstuck */
  if(CheckCollision(nearby, 0, 0))
  {
    Unstuck(nearby, 20);
  }

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

/* Collision */
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
  if(CheckCollision(nearby, position.x, position.y))
  {
    for(int i = 0; i < limit; i++)
    {
      // Right
      if(!CheckCollision(nearby, position.x + i, position.y))
      {
        position.x+=i;
        break;
      }
      // Left
      if(!CheckCollision(nearby, position.x - i, position.y))
      {
        position.x-=i;
        break;
      }
        
      // Up
      if(!CheckCollision(nearby, position.x, position.y-i))
      {
        position.y-=i;
        break;
      }
      // Down
      if(!CheckCollision(nearby, position.x, position.y+i))
      {
        position.y+=i;
        break;
      }
      
      //Top Right
      if(!CheckCollision(nearby, position.x+i, position.y-i))
      {
        position.x+=i;
        position.y-=i;
        break;
      }
      //Top Left
      if(!CheckCollision(nearby, position.x-i, position.y-i))
      {
        position.x-=i;
        position.y-=i;
        break;
      }
        
      //Bottom Right
      if(!CheckCollision(nearby, position.x+i, position.y+i))
      {
        position.x+=i;
        position.y+=i;
        break;
      }
        
      //Bottom Left
      if(!CheckCollision(nearby, position.x-i, position.y+i))
      {
        position.x-=i;
        position.y+=i;
        break;
      }
    }
  }
}
