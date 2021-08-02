#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>

class EBO
{
public:
	// Reference ID
	GLuint ID;
	// Constructor, generate and bind EBO to indices
	EBO(GLuint* indices, GLsizeiptr size);
	// Bind EBO
	void Bind();
	// Unbind EBO
	void Unbind();
	// Delete EBO
	void Delete();
};

#endif
