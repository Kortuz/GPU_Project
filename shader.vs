#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;

out vec2 texCoord;

uniform mat4 camMatrix;
uniform float scale;

void main()
{
    // Output Position
    gl_Position = camMatrix * vec4(aPos.x + scale, aPos.y + scale, aPos.z + scale, 1.0);
    texCoord = aTex;
}


