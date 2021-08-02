#include "EBO.h"

//Constructor, generate and bind EBO to indices
EBO::EBO(GLuint* indices, GLsizeiptr size)
{
	// Generate EBO 
	glGenBuffers(1, &ID);
	// Bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	// Indices to EBO 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

// Bind EBO
void EBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// Unbind EBO
void EBO::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Delete EBO
void EBO::Delete()
{
	glDeleteBuffers(1, &ID);
}