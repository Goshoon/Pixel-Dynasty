#pragma once
#include "pixel.h"
#include <fstream>
#include <vector>

void SaveGame(std::vector<Pixel>& game);
void LoadGame(std::vector<Pixel>& game);