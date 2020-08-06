#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

#define WIN_WIDTH	640
#define WIN_HEIGHT	480
#define WIN_NAME	"Test"

const char* vertex_shader = " \
	#version 330 core \n\
	layout(location = 0) in vec3 a_pos; \n\
	void main() { \n\
		gl_Position = vec4(a_pos, 1.0); \n\
	\n} \
";

const char* fragment_shader = " \
	#version 330 core \n\
	layout(location = 0) out vec4 FragColor; \n\
	uniform vec4 u_color; \n\
	void main() { \n\
	    FragColor = u_color; \n\
	\n} \
";

void temp_error_callback(int error_code, const char* descr) {
	std::cout << "GLFW Error: " << error_code << " " << descr << std::endl;
}

void temp_key_callback(GLFWwindow *wind, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(wind, GL_TRUE);
	}

	std::cout << "Event with keycode: " << key  << action  << " " << scancode << " " << mods << std::endl; 
}

void draw_loop(GLFWwindow *window) {
	glfwMakeContextCurrent(window);

	Shader demo_shader(vertex_shader, fragment_shader);
	const float triangle_color[4] = {1.0f, 1.0f, 0.0f, 1.0f};
	const float clip_vertex[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	// Send teh vertex to the GPU via Vertex Buffer Objects
	unsigned int vbo, vao;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(clip_vertex), clip_vertex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	while(!glfwWindowShouldClose(window)) {
		// Draw loop
		int width, heigth;
		
		glfwGetFramebufferSize(window, &width, &heigth);
		// Set to OpenGL viewport size anc coordinates
		glViewport(0,0, width, heigth);

		// OpenGL stuff
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.5f, 0.0f, 0.5f, 1.0f);

		demo_shader.activate();
		demo_shader.set_uniform_vector("u_color", triangle_color);

		glBindVertexArray(vbo);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

int main() {
	if (!glfwInit()) {
		return EXIT_FAILURE;
	}
	
	// GLFW config
	glfwSetErrorCallback(temp_error_callback);

	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_NAME, NULL, NULL);

	glfwSetKeyCallback(window, temp_key_callback);

	glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	glfwSwapInterval(1);
	
	if (!window) {
		std::cout << "Error, could not create window" << std::endl; 
	} else {
		draw_loop(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
