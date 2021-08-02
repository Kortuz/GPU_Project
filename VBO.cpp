#include "VBO.h"

// Constructor (bind and link to vertices)
VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
	// Generate VBO 
	glGenBuffers(1, &ID);
	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	// Vertices to VBO 
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

// Bind VBO
void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Unbind VBO
void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Delete VBO
void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}