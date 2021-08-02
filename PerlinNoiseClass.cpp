#include "PerlinNoiseClass.h"

PerlinNoise::PerlinNoise(int widthP, int heightP, int widthG, int heightG)
{
	PerlinNoise::widthP = widthP;
	PerlinNoise::heightP = heightP;
	PerlinNoise::widthG = widthG;
	PerlinNoise::heightG = heightG;
}


float* PerlinNoise::run(glm::vec2 *gradGrid, float* heightMap)
{
	extern float* runPerlinNoise(glm::vec2 *gradGrid, float* heightMap, int noiseW, int noiseH, int gradW, int gradH);
	* heightMap = * runPerlinNoise(gradGrid, heightMap, widthP, heightP, widthG, heightG);
	return heightMap;
}

float* PerlinNoise::Interpolate(float* hMT, float* hM1, float* hM2, float* hM3, float dt)
{
	extern float* runInterpolate(float* hMT, float* hM1, float* hM2, float* hM3, int noiseSize, float dt);
	*hMT = * runInterpolate(hMT, hM1, hM2, hM3, widthP * heightP, dt);
	return hMT;
}