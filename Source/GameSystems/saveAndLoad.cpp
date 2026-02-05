#include "saveAndLoad.h"
#include <iostream>
void SaveGame(std::vector<Pixel>& game)
{
	std::ofstream file("save.data");
    if (!file.is_open()) return;

    file << game.size() << "\n";

    for (const auto& p : game) 
    {
    	p.save(file);
    }
}

void LoadGame(std::vector<Pixel>& game)
{
	std::ifstream file("save.data");
    if (!file.is_open()) return;

	size_t count;
    file >> count;
    std::cout << "LoadGame: loading " << count << " pixels\n";

    game.clear();
    game.reserve(count);

    for (size_t i = 0; i < count; i++)
    {
        Pixel p;
        p.load(file);
        game.push_back(p);
    }
}