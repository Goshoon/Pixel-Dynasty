// https://www.youtube.com/watch?v=iUq0waTh9Pw&ab_channel=%E2%98%86111loggedin%2F%2Fxxenaa%E2%99%A1
#include "pixel.h"
#include <iostream>

Pixel::Pixel()
{
  lifetime = 0;
  position.w = 1;
  position.h = 1;
  worldBorder.x = WINDOW_WIDTH / RENDER_SCALE;
  worldBorder.y = WINDOW_HEIGHT / RENDER_SCALE - 1;
}

Pixel::Pixel(int x, int y, Color col)
{
  lifetime = 0;
  position.x = x;
  position.y = y;
  position.w = 1;
  position.h = 1;
  color = col;
  worldBorder.x = WINDOW_WIDTH / RENDER_SCALE;
  worldBorder.y = WINDOW_HEIGHT / RENDER_SCALE - 1;
}

void Pixel::Update(std::vector<Pixel*>& nearby)
{
  if (material != STATIC && CheckCollision(nearby, 0, 0))
    Unstuck(nearby, 3);

  if (position.x < -1000 || position.x > worldBorder.x + 1000 || position.y < -1000 || position.y > worldBorder.y + 1000)
  {
    std::cerr << "Pixel::Update abnormal pos (" << position.x << "," << position.y << ") resetting to bounds\n";
    position.x = Clamp(position.x, 0, worldBorder.x);
    position.y = Clamp(position.y, 0, worldBorder.y);
  }

  /* Particle behaviours */
  switch(material)
  {
    default:
      break;

    case STONE:
    case DYNAMIC:
      Gravity(nearby);
      break;

    case ASH:
      Gravity(nearby);

      lifetime++;
      if (lifetime > 900)
        destroy = true;
      break;

    case WOOD:
      /* Other material reactions */
      if (CheckCollision(nearby, 0, -1, FIRE))
      {
        material = ASH;
        Color tmp;
        color = GetMaterialColor(tmp, ASH);
      }
      if (CheckCollision(nearby, 0, 1, FIRE))
      {
        material = ASH;
        Color tmp;
        color = GetMaterialColor(tmp, ASH);
      }
      break;

    case FIRE:
      GasBehaviour(nearby);

      /* Other material reactions */
      if (CheckCollision(nearby, 0, -1, WATER) || CheckCollision(nearby, 0, 1, WATER))
      {
        destroy = true;
      }

      /* Gas Lifetime - fade out less frequently */
      if (lifetime > 300 && lifetime % 2 == 0)
        color.alpha--;

      if (color.alpha < 100)
        destroy = true;
    break;
    case STEAM:
      GasBehaviour(nearby);

      if (lifetime > 100 && lifetime % 2 == 0)
        color.alpha-=2;

      if (color.alpha < 100)
        destroy = true;
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
            static thread_local std::mt19937 gen(std::random_device{}());
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
          else if (!leftBellowCol && position.x > 0 && position.y < worldBorder.y)
          {
            position.x--;
            position.y++;
          }
          else if (!rightBellowCol && position.x < worldBorder.x && position.y < worldBorder.y)
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
          material = STEAM;
          Color tmp;
          color = GetMaterialColor(tmp, STEAM);
        }
        if (CheckCollision(nearby, 0, 1, FIRE))
        {
          material = STEAM;
          Color tmp;
          color = GetMaterialColor(tmp, STEAM);
        }

        /* Change alpha less frequently to reduce overhead */
        if (lifetime % 3 == 0)
        {
          static thread_local std::mt19937 gen(std::random_device{}());
          std::uniform_int_distribution<> dist(100, 255);
          color.alpha = dist(gen);
        } 
    }
    break;

    case DIRT:
    {
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

    case DYNAMITE:
    {      
      if (!ignited)
      {
        if (CheckCollision(nearby, 0, -1, FIRE) ||
          CheckCollision(nearby, 0, 1, FIRE) ||
          CheckCollision(nearby, -1, 0, FIRE) ||
          CheckCollision(nearby, 1, 0, FIRE))
        {
          ignited = true;
          fuseTimer = 60; // 1 second if 60fps
          color.red = 255; // visual feedback
        }
      }
      else
      {
        fuseTimer--;

        // Blink effect
        if (fuseTimer % 10 < 5)
          color = {255, 0, 0, 255};
        else
          color = {200, 0, 0, 255};

        if (fuseTimer <= 0)
        {
          Explode(nearby);
          destroy = true;
        }
      }
      break;
    }
  }

  /* Stay within world limits */
  position.x = Clamp(position.x, 0, worldBorder.x);
  position.y = Clamp(position.y, 0, worldBorder.y);
}

void Pixel::Draw()
{
  Application& app = Application::GetInstance();
  if (position.x < 0 || position.x > worldBorder.x || position.y < 0 || position.y > worldBorder.y)
  {
    std::cerr << "Warning: Pixel::Draw out-of-bounds pos(" << position.x << "," << position.y << ") material=" << static_cast<int>(material) << "\n";
    return;
  }
  SDL_SetRenderDrawColor(app.renderer, color.red, color.green, color.blue, color.alpha);
  SDL_RenderDrawPoint(app.renderer, position.x, position.y);
}

/* Gravity */
void Pixel::Gravity(std::vector<Pixel*>& nearby)
{
  if (position.y < WINDOW_HEIGHT / RENDER_SCALE - 1)
  {
    bool canGoDown = true;

    if (CheckCollision(nearby, 0, 1))
      canGoDown = false;

    if (!canGoDown)
    {
      Unstuck(nearby, 3);
    }
    else
    {
      position.y++;
    }
  }
}

/* Explosive actions */
void Pixel::Explode(std::vector<Pixel*>& nearby)
{
    int radius = 8;

    for (auto& near : nearby)
    {
        int dx = near->position.x - position.x;
        int dy = near->position.y - position.y;

        if (dx*dx + dy*dy <= radius*radius)
        {
            near->destroy = true;

            // Optional: turn some into fire
            if (rand() % 3 == 0)
            {
                near->material = FIRE;
                Color tmp;
                near->color = GetMaterialColor(tmp, FIRE);
            }
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

bool Pixel::CheckCollision(std::vector<Pixel*>& nearby, int xoffset, int yoffset, Material materialLookup)
{
  SDL_Rect pos = position;
  pos.x += xoffset;
  pos.y += yoffset;

  for (auto& near : nearby)
  {
    if (this == near) continue;

    if (Collide(pos, near->position) && near->material == materialLookup)
      return true;
  }

  return false;
}

/* Unstuck */
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
      static thread_local std::mt19937 gen(std::random_device{}());
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