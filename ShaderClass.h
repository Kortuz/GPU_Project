#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
	public:
		// Reference ID
		GLuint ID;
		// Constructor, create a shader program with vertex and fragment
		Shader(const char* vertexFile, const char* fragmentFile);
		// Activate shader program
		void Activate();
		// Delete shader program
		void Delete();

	private:
		// Check shader compile and linking errors
		void CompileErrors(unsigned int shader, const char* type);
};

#endif
