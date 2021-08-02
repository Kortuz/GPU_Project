#include"shaderClass.h"

// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

// Constructor, create a shader program with vertex and fragment
Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

    // Load vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Passing source code
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    // Compiling shader
    glCompileShader(vertexShader);
    CompileErrors(vertexShader, "VERTEX");

    // Load fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Passing source code
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    // Compiling shader
    glCompileShader(fragmentShader);
    CompileErrors(fragmentShader, "FRAGMENT");

    // Create Shader program (wrapper for vertex and fragment)
    ID = glCreateProgram();
    // Attach vertex shader
    glAttachShader(ID, vertexShader);
    // Attach fragment shader
    glAttachShader(ID, fragmentShader);
    // Linking
    glLinkProgram(ID);
    CompileErrors(ID, "PROGRAM");

    // Delete previous shader (already loaded in shader program)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// Activate shader program
void Shader::Activate()
{
	glUseProgram(ID);
}

// Delete shader program
void Shader::Delete()
{
	glDeleteProgram(ID);
}

// Check shader compile and linking errors
void Shader::CompileErrors(unsigned int shader, const char* type)
{
    GLint hasCompiled;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "SHADER_COMPILATION_ERROR: " << type << "\n" << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "SHADER_LINKING_ERROR: " << type << "\n" << std::endl;
        }
    }
}