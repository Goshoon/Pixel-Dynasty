#pragma once
#include <vector>
#include <memory>
#include <SDL2/SDL.h>
#include "pixel.h"

class Quadtree
{
    static const int MAX_ENTITIES = 8;
    static const int MAX_LEVELS = 4;

    int level;
    std::vector<Pixel*> pixels;
    std::vector<std::unique_ptr<Quadtree>> nodes;
public:
    Quadtree(int level, SDL_Rect bounds);
    SDL_Rect bounds;

    void Clear();
    void Split();
    int GetIndex(const SDL_Rect& rect) const;
    void Insert(Pixel* pixel);
    void Retrieve(std::vector<Pixel*>& returnPixels, const SDL_Rect& area);
    void Draw(SDL_Renderer* renderer) const;
};