#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <math.h>
#include <time.h>
#include <iostream>
#include <vector>
#include<assert.h>
#include <random>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb/stb_image.h>

#include <cuda_runtime.h>
#include <cuda_gl_interop.h>

#include "TextureClass.h"
#include "ShaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "CameraClass.h"
#include "PerlinNoiseClass.h"

// Resolution
const GLsizei width = 1000; 
const GLsizei height = 1000;
const GLint resolution[] = { width, height };

const int noiseW = 1024;
const int noiseH = 1024;

const int gradW = 1024;
const int gradH = 1024;


int main() {


    // Initialize GLFW
    glfwInit();
    // Set glfw for OpenGl 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // CORE profile, modern functions only
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Check window creation
    GLFWwindow* window = glfwCreateWindow(width, height, "OwO", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed creating glfw window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Telling GLFW the window
    glfwMakeContextCurrent(window);

    // Load glad to config openGL
    gladLoadGL();
    // Viewport of OpenGL in the Window
    glViewport(0, 0, width, height);

    // Creating  height mao for i-1, i and i+1 time
    PerlinNoise pNoise(noiseW, noiseH, gradW, gradH);
    float* hM = (float*)malloc(sizeof(float) * noiseW * noiseW);
    glm::vec2* gG = (glm::vec2*) malloc(sizeof(glm::vec2) * noiseW * noiseH);
    pNoise.run(gG, hM);

    float* hM2 = (float*)malloc(sizeof(float) * noiseW * noiseH);
    glm::vec2* gG2 = (glm::vec2*) malloc(sizeof(glm::vec2) * noiseW * noiseH);
    pNoise.run(gG2, hM2);

    float* hM3 = (float*)malloc(sizeof(float) * noiseW * noiseH);
    glm::vec2* gG3 = (glm::vec2*) malloc(sizeof(glm::vec2) * noiseW * noiseH);
    pNoise.run(gG3, hM3);

    // Interpolate height map
    float* hMT = (float*)malloc(sizeof(float) * noiseW * noiseH);

    // Texture screen
    GLfloat screenVertices[] =
    { //     Coordinate     /        Color      /   Texture   //
        -1.0f, -1.0f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
        -1.0f,  1.0f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
         1.0f,  1.0f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
         1.0f, -1.0f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
    };

    // Indices
    GLuint indices[] =
    {
        0, 2, 1, // Upper triangle
        0, 3, 2 // Lower triangle
    };


    // Create shader program
    Shader shaderProgram("shader.vs", "shader.fs");

    // Create VAO
    VAO VAO1;
    // Bind VAO as the current one
    VAO1.Bind();

    // Create VBO and EBO
    VBO VBO1(screenVertices, sizeof(screenVertices));
    EBO EBO1(indices, sizeof(indices));

    // Link VBO to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    // Unbind VAO, VBO, EBO 
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Height map texture
    GLuint hmap[1];
    glGenTextures(1, hmap);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hmap[0]);
    // Type of processing
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, noiseW, noiseH, 0, GL_RED, GL_FLOAT, hMT);

    // Enable depth
    glEnable(GL_DEPTH_TEST);

    // Camera
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    int im = 0;
    double dt = 0;
    double prevT = glfwGetTime();

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Color Background
        glClearColor(0.102f, 0.0f, 0.051f, 1.0f);
        // Clean buffer and change the color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Tell OpenGL to use shader program
        shaderProgram.Activate();

        camera.Inputs(window);
        camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

        // Tell OpenGL to use VAO
        VAO1.Bind();

        // Draw primitives
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

        VAO1.Unbind();
        double Ktime = 0;
        double crrtT = glfwGetTime();
        /*if (double(10) < crrtT - prevT) { // Couldn't connect well the frames when a new height map is created
            *hM = *hM2;
            *hM2 = *hM3;
            Ktime = glfwGetTime();
            pNoise.run(gG3, hM3);
            Ktime = glfwGetTime() - Ktime;

            prevT = crrtT;

            pNoise.Interpolate(hMT, hM, hM2, hM3, dt);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, noiseW, noiseH, 0, GL_RED, GL_FLOAT, hMT);

            glActiveTexture(GL_TEXTURE0+1);
            glBindTexture(GL_TEXTURE_2D, hmap[0]);
        }
        else
        {*/
           // dt = crrtT - prevT - Ktime;
            dt = crrtT - prevT;
            pNoise.Interpolate(hMT, hM, hM2, hM3 , dt);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, noiseW, noiseH, 0, GL_RED, GL_FLOAT, hMT);

            glActiveTexture(GL_TEXTURE0+1);
            glBindTexture(GL_TEXTURE_2D, hmap[0]);
        //}

        // Swap buffers 
        glfwSwapBuffers(window);

        // GLFW events
        glfwPollEvents();
    }

    // Delete VAO, VBO and EBO
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();

    // Delete shader program
    shaderProgram.Delete();

    // Delete window
    glfwDestroyWindow(window);
    // Terminate GLFW
    glfwTerminate();
    return 0;
}

