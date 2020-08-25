#include "shader.h"
#include <iostream>

// To do: Show programming errors
sShader::sShader(const char* vertex_shader_raw, 
               const char* fragment_shader_raw) {
    int vertex_id, fragment_id;
    int compile_successs;
    char compile_log[512];

    // Create and compile the shaders
    vertex_id = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertex_id, 1, &vertex_shader_raw, NULL);
    glCompileShader(vertex_id);
    glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &compile_successs);

    if (!compile_successs) {
        glGetShaderInfoLog(vertex_id, 512, NULL, compile_log);
        std::cout << compile_log << std::endl;
        assert(">>>>>Error comiling vertex shader" && false);
    }
    
    fragment_id = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragment_id, 1, &fragment_shader_raw, NULL);
    glCompileShader(fragment_id);
    glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &compile_successs);

    if (!compile_successs) {
        glGetShaderInfoLog(fragment_id, 512, NULL, compile_log);
        std::cout << compile_log << std::endl;
        assert(">>>>>Error comiling fragment shader" && false);
    }

    // Create the shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertex_id);
    glAttachShader(ID, fragment_id);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &compile_successs);

    if (!compile_successs) {
        glGetProgramInfoLog(ID, 512, NULL, compile_log);
        std::cout << compile_log << std::endl;
        assert(">>>>>Shader Linking error" && false);
    }

    // Cleanup
    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);
}

void sShader::activate() {
    glUseProgram(ID);
}

void sShader::deactivate() {
    int i = 0;
}

void sShader::set_uniform(const char* name, 
                        const float value) const {
    glUniform1f(glGetUniformLocation(ID, name), value);

}
void sShader::set_uniform(const char* name, 
                        const int value) const {
    glUniform1i(glGetUniformLocation(ID, name), value);
}

void sShader::set_uniform(const char* name, 
                        const bool value) const {
    glUniform1i(glGetUniformLocation(ID, name), (int) value);
}

void sShader::set_uniform_vector(const char* name,
                        const float value[4]) const {
    glUniform4fv(glGetUniformLocation(ID, name), 1, value);
}