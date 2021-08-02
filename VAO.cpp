#include "VAO.h"

// Constructor, generate VAO
VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

// Bind to VBO, enable this VAO and unbind the VBO
void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	// Config VBO
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);	
	// Enable VAO
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

// Binding VAO and making it current VAO
void VAO::Bind()
{
	glBindVertexArray(ID);
}

// Unbind VAO
void VAO::Unbind()
{
	glBindVertexArray(0);
}

// Delete VAO
void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}