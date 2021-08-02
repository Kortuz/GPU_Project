#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "device_atomic_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <random>

#include <math.h>
#include <glm/glm.hpp>
#include <iostream>

# define M_PI 3.14159265358979323846  /* pi */

std::random_device rd; // obtain a random number from hardware
std::mt19937 gen(rd()); // seed the generator
std::uniform_real_distribution<> distr(-1, 1);

__device__ float fade(float t) {
    return t*t*(3.0-2.0*t); // Old fade
    //return t * t * t * (t * (t * 6.0 - 15.0) + 10.0); // Improved fade
}

__global__ void perlinNoise(glm::vec2 *gradGrid, float *heightMap, int gradWidth, int gradHeight, int hmapWidth, int hmapHeight, float frequency, float amplitude)
{
    int gradSize = (gradWidth * gradHeight);

    float hx = ((threadIdx.x + blockDim.x * blockIdx.x) % hmapWidth);
    float hy = ((threadIdx.x + blockDim.x * blockIdx.x) / hmapWidth);
    
    glm::vec2 p = glm::vec2(frequency * (float) hx / (float) gradWidth, frequency * (float) hy / (float) gradHeight);

    glm::vec2 w = glm::vec2(p.x - floor(p.x), p.y - floor(p.y));

    glm::vec2 downLeft = glm::vec2(floor(p.x), floor(p.y));
    glm::vec2 downRight = glm::vec2(downLeft.x + 1.0f, downLeft.y + 0.0f);
    glm::vec2 upLeft = glm::vec2(downLeft.x + 0.0f, downLeft.y + 1.0f);
    glm::vec2 upRight = glm::vec2(downLeft.x + 1.0f, downLeft.y + 1.0f);

    glm::vec2 g0 = gradGrid[((int)(downLeft.x + gradWidth * downLeft.y) % gradSize)];
    glm::vec2 g1 = gradGrid[((int)(downRight.x + gradWidth * downRight.y) % gradSize)];
    glm::vec2 g2 = gradGrid[((int)(upLeft.x + gradWidth * upLeft.y) % gradSize)];
    glm::vec2 g3 = gradGrid[((int)(upRight.x + gradWidth * upRight.y) % gradSize)];

    float up = (1.0 - fade(w.x)) * dot(g0, glm::vec2(p.x - downLeft.x, p.y - downLeft.y)) + fade(w.x) * dot(g1, glm::vec2(p.x - downRight.x, p.y - downRight.y));
    float down = (1.0 - fade(w.x)) * dot(g2, glm::vec2(p.x - upLeft.x, p.y - upLeft.y)) + fade(w.x) * dot(g3, glm::vec2(p.x - upRight.x, p.y - upRight.y));

    heightMap[(threadIdx.x + blockDim.x * blockIdx.x)] += ((1.0 - fade(w.y)) * up + fade(w.y) * down) * amplitude;


}

float* runPerlinNoise(glm::vec2 *gradGrid, float *heightMap, int noiseW, int noiseH, int gradW, int gradH)
{

    float *d_hMap;
    glm::vec2 *d_gGrid;

    for (int i = 0; i < gradW * gradH; i++)
    {
        gradGrid[i].x = distr(gen);
        gradGrid[i].y = distr(gen);
    }

    cudaMalloc(&d_hMap, sizeof(float) * noiseW * noiseH);
    cudaMalloc(&d_gGrid, sizeof(glm::vec2) * gradW * gradH);

    cudaMemcpy(d_gGrid, gradGrid, gradW * gradH * sizeof(glm::vec2), cudaMemcpyHostToDevice);
    int N = noiseH * noiseW;

    // Run kernel for each octave
    for (int i = 0; i < 10; i++)
    {
        perlinNoise << <(N + 255) / 256, 256 >> > (d_gGrid, d_hMap, noiseW, noiseH, gradW, gradH, pow(2.0f, i), pow(0.99f, i));
        cudaDeviceSynchronize();
        cudaMemcpy(heightMap, d_hMap, noiseW * noiseH * sizeof(float), cudaMemcpyDeviceToHost);
    }

    cudaFree(d_hMap);
    cudaFree(d_gGrid);

    return heightMap;
}

__global__ void interpolate(float* d_hMT, float* hM1, float* hM2, float* hM3, float dt)
{
    float w1 = cos((M_PI / 3) * (dt + 2));
    float w2 = cos((M_PI / 3) * (dt + 1));
    float w3 = cos((M_PI / 3) * (dt));

    float inter = (w1 * hM1[(threadIdx.x + blockDim.x * blockIdx.x)]) + (w2 * hM2[(threadIdx.x + blockDim.x * blockIdx.x)]) + (w3 * hM3[(threadIdx.x + blockDim.x * blockIdx.x)]);

    d_hMT[(threadIdx.x + blockDim.x * blockIdx.x)] = inter;
}

float* runInterpolate(float* hMT, float* hM1, float* hM2, float* hM3, int noiseSize, float dt)
{
    float* d_hMT;
    float* d_hM1;
    float* d_hM2;
    float* d_hM3;

    cudaMalloc(&d_hMT, sizeof(float) * noiseSize);
    cudaMalloc(&d_hM1, sizeof(float) * noiseSize);
    cudaMalloc(&d_hM2, sizeof(float) * noiseSize);
    cudaMalloc(&d_hM3, sizeof(float) * noiseSize);

    cudaMemcpy(d_hM1, hM1, sizeof(float) * noiseSize, cudaMemcpyHostToDevice);
    cudaMemcpy(d_hM2, hM2, sizeof(float) * noiseSize, cudaMemcpyHostToDevice);
    cudaMemcpy(d_hM3, hM3, sizeof(float) * noiseSize, cudaMemcpyHostToDevice);

    interpolate << <(noiseSize + 255) / 256, 256 >> > (d_hMT, d_hM1, d_hM2, d_hM3, dt);

    cudaMemcpy(hMT, d_hMT, sizeof(float) * noiseSize, cudaMemcpyDeviceToHost);

    cudaFree(d_hMT);
    cudaFree(d_hM1);
    cudaFree(d_hM2);
    cudaFree(d_hM3);
    return hMT;
}