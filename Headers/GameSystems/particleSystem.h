#pragma once
#include <vector>
#include "color.h"

struct Particle
{
	float x, y;
	float vx, vy;
	Color color;
	float lifetime;
	float maxLifetime;
};

class ParticleSystem
{
public:
	void Update();
	void Draw();
	void Emit(float x, float y, int count, Color col, float vx, float vy);
private:
	std::vector<Particle> particles;
};