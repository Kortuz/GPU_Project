#include "TextureClass.h"

// Constructor
Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
    // Setting texture type
    type = texType;
    // Image values
    int widthImg, heightImg, numColCh;
    // stb loves loading flipped images
    stbi_set_flip_vertically_on_load(true);
    // Load image
    unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

    // Generate texture
    glGenTextures(1, &ID);
    // Assign slot
    glActiveTexture(slot);
    // Bind texture
    glBindTexture(type, ID);

    // Type of processing
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Type of repeat (s==x) (t==y) (r==z)
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // Pass image to texture object
    glTexImage2D(type, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
    glGenerateMipmap(type);

    // Free and unbind image
    stbi_image_free(bytes);
    glBindTexture(type, 0);
}

// Pass texture to shader
void Texture::textUnit(Shader& shader, const char* uniform, GLuint unit)
{
    // Uniform location
    GLuint texUni = glGetUniformLocation(shader.ID, uniform);
    // Activate shader in order to be able to change uniform value
    shader.Activate();
    // Change uniform value
    glUniform1i(texUni, unit);
}

// Bind texture
void Texture::Bind()
{
    glBindTexture(type, ID);
}

// Unbind texture
void Texture::Unbind()
{
    glBindTexture(type, 0);
}

// Delete texture
void Texture::Delete()
{
    glad_glDeleteTextures(1, &ID);
}