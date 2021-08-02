#version 330 core
#define MAX_SIZE_ARRAY 1000

out vec4 FragColor;

#ifdef GL_ES
precision mediump float;
#endif

in vec2 texCoord;

uniform  sampler2D tex0;

void main()
{
    //FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    FragColor = texture(tex0, texCoord).rrra;
} 