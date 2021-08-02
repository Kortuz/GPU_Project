#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "shaderClass.h"

class Texture
{
	public:
		// Reference ID
		GLuint ID;
		// Image type
		GLenum type;
		// Constructor
		Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

		// Pass texture to shader
		void textUnit(Shader& shader, const char* uniform, GLuint unit);
		// Bind texture
		void Bind();
		// Unbind texture
		void Unbind();
		// Delete texture
		void Delete();
};
#endif
