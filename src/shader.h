#ifndef _SHADER_H_
#define _SHADER_H_

#include <glad/glad.h>
#include <stddef.h>
#include <cassert>

/**
 * Basic OpenGL Shader Class, for shader's IO boilerplate
 * Juan S. Marquerie
*/
class Shader {
public:
    unsigned int ID;

    Shader(const char* vertex_shader, const char* fragment_shader);

    void activate();
    void deactivate();

    // Setters for the shader's uniforms
    void set_uniform(const char* name, const float value) const;
    void set_uniform(const char* name, const int value) const;
    void set_uniform(const char* name, const bool value) const;
    void set_uniform_vector(const char* name, const float value[4]) const;
};

#endif