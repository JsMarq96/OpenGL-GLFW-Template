#version 330 core

layout (location = 0) in vec3 a_vertex;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_normal;

uniform mat4 u_prev_model;
uniform mat4 u_prev_viewprojection;

uniform mat4 u_model_mat;
uniform mat4 u_viewproj_mat;

uniform vec3 u_light_position;

//this will store the color for the pixel shader
varying vec3 v_world_position;
varying vec4 v_clip_position;
varying vec4 v_prev_clip_position;
varying vec3 v_normal;
varying vec2 v_uv;

void main() {	
	//calcule the normal in camera space (the NormalMatrix is like ViewMatrix but without traslation)
	v_normal = (u_model_mat * vec4( a_normal, 0.0) ).xyz;
	
	//calcule the vertex in object space
	vec4 world_position = (u_model_mat * vec4(a_vertex, 1.0) );

	vec4 pos = u_viewproj_mat * u_model_mat * vec4(a_vertex, 1.0);

	v_clip_position = u_viewproj_mat * world_position;

	v_world_position = world_position.xyz;
	//v_prev_clip_position = (u_prev_viewprojection * u_prev_model * vec4(a_vertex, 1.0));

    v_uv = a_uv;

	//calcule the position of the vertex using the matrices
	gl_Position = pos;
}
