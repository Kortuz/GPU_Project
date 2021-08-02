#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>

class VBO
{
	public:
		// Reference ID
		GLuint ID;
		// Constructor (bind VBO and link to vertices)
		VBO(GLfloat* vertices, GLsizeiptr size);
		// Bind the VBO 
		void Bind();
		// Unbind the VBO
		void Unbind();
		// Delete the VBO
		void Delete();
};

#endif
