#version 330 core

layout(location = 0) in vec3 a_v_pos;

out vec3 tex_coords;

uniform mat4 u_model_mat;
uniform mat4 u_viewproj_mat;

void main() {
	tex_coords = a_v_pos;

	vec4 pos = u_viewproj_mat * u_model_mat * vec4(a_v_pos, 1.0);
	gl_Position = pos.xyww;
}