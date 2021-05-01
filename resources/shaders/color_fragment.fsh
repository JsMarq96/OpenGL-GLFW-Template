#version 330 core

layout(location = 0) out vec4 o_frag_color;

uniform vec4 u_color;

void main() {
    o_frag_color = vec4(1.0,1.0,1.0,1.0);//u_color;
}
