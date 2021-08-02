#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.h"

class VAO
{
	public:
		// Reference ID
		GLuint ID;
		// Constructor, generate VAO
		VAO();
		// Bind to VBO, enable this VAO and unbind the VBO
		void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
		// Binding VAO and making it current VAO
		void Bind();
		// Unbind VAO
		void Unbind();
		// Delete VAO
		void Delete();
};

#endif
