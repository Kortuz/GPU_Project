#pragma once
#include <vector_types.h>
#include <stdlib.h>
#include <time.h>
#include <glm/glm.hpp>
#include <iostream>

class PerlinNoise
{
	public:
		int widthP;
		int heightP;
		int widthG;
		int heightG;

		float* heightMap;

		PerlinNoise(int widthP, int heightP, int widthV, int heightV);

		float* run(glm::vec2* gradGrid, float* heightMap);
		float* Interpolate(float* hMT, float* hM1, float* hM2, float* hM3, float dt);
};

