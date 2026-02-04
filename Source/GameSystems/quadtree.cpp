#include "quadtree.h"

Quadtree::Quadtree(int level, SDL_Rect bounds)
    : level(level), bounds(bounds) {}

void Quadtree::Clear()
{
    pixels.clear();
    for (auto& node : nodes)
    {
        if (node) node->Clear();
    }
    nodes.clear();
}

void Quadtree::Split()
{
    int subWidth = bounds.w / 2;
    int subHeight = bounds.h / 2;
    int x = bounds.x;
    int y = bounds.y;

    if (subWidth == 0 || subHeight == 0)
        return;

    nodes.emplace_back(std::make_unique<Quadtree>(level + 1, SDL_Rect{x + subWidth, y, bounds.w - subWidth, subHeight}));
    nodes.emplace_back(std::make_unique<Quadtree>(level + 1, SDL_Rect{x, y, subWidth, subHeight}));
    nodes.emplace_back(std::make_unique<Quadtree>(level + 1, SDL_Rect{x, y + subHeight, subWidth, bounds.h - subHeight}));
    nodes.emplace_back(std::make_unique<Quadtree>(level + 1, SDL_Rect{x + subWidth, y + subHeight, bounds.w - subWidth, bounds.h - subHeight}));
} 

int Quadtree::GetIndex(const SDL_Rect& rect) const
{
    int verticalMidpoint = bounds.x + bounds.w / 2;
    int horizontalMidpoint = bounds.y + bounds.h / 2;

    bool top = rect.y + rect.h <= horizontalMidpoint;
    bool bottom = rect.y >= horizontalMidpoint;
    bool left = rect.x + rect.w <= verticalMidpoint;
    bool right = rect.x >= verticalMidpoint; 

    if (top && right) return 0;
    if (top && left) return 1;
    if (bottom && left) return 2;
    if (bottom && right) return 3;

    return -1;  // doesn't fit neatly into a quadrant
}

void Quadtree::Insert(Pixel* pixel) 
{
    if (!nodes.empty())
    {
        int index = GetIndex(pixel->position);
        if (index != -1)
        {
            nodes[index]->Insert(pixel);
            return;
        }
    }

    pixels.push_back(pixel);

    if (pixels.size() > MAX_ENTITIES && level < MAX_LEVELS)
    {
        if (nodes.empty()) Split();

        auto it = pixels.begin();
        while (it != pixels.end())
        {
            int index = GetIndex((*it)->position);
            if (index != -1)
            {
                nodes[index]->Insert(*it);
                it = pixels.erase(it);
            } 
            else
            {
                ++it;
            }
        }
    }
}

void Quadtree::Retrieve(std::vector<Pixel*>& returnPixels, const SDL_Rect& area) 
{
    if (!SDL_HasIntersection(&bounds, &area))
        return;

    if (!nodes.empty()) {
        for (const auto& node : nodes) {
            if (SDL_HasIntersection(&node->bounds, &area)) {
                node->Retrieve(returnPixels, area);
            }
        }
    }

    // Add all objects in this node
    returnPixels.insert(returnPixels.end(), pixels.begin(), pixels.end());
} 

void Quadtree::Draw(SDL_Renderer* renderer) const
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 190, 100);
    SDL_RenderDrawRect(renderer, &bounds);

    for (const auto& node : nodes) 
    {
        if (node) node->Draw(renderer);
    }
}
