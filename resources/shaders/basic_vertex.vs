#version 330 core

layout(location = 0) in vec3  i_pos;

//uniform mat3 u_model;

void main() {
    // TODO: add scalling and centering
    // TODO: Model matrix really needed...?
    gl_Position = vec4(i_pos, 0.0);
}
