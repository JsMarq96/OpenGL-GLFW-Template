#include <iostream>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "texture.h"
#include "mesh.h"
#include "material.h"
#include "mesh_renderer.h"
#include "shader.h"
#include "input_layer.h"

// Dear IMGUI
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#define WIN_WIDTH	640
#define WIN_HEIGHT	480
#define WIN_NAME	"Test"

#define PI 3.14159265359f

void temp_error_callback(int error_code, const char* descr) {
	std::cout << "GLFW Error: " << error_code << " " << descr << std::endl;
}

// INPUT MOUSE CALLBACk
void key_callback(GLFWwindow *wind, int key, int scancode, int action, int mods) {
	// ESC to close the game
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(wind, GL_TRUE);
	}

	eKeyMaps pressed_key;
	switch(key) {
		case GLFW_KEY_W:
			pressed_key = W_KEY;
			break;
		case GLFW_KEY_A:
			pressed_key = A_KEY;
			break;
		case GLFW_KEY_S:
			pressed_key = S_KEY;
			break;
		case GLFW_KEY_D:
			pressed_key = D_KEY;
			break;
		case GLFW_KEY_UP:
			pressed_key = UP_KEY;
			break;
		case GLFW_KEY_DOWN:
			pressed_key = DOWN_KEY;
			break;
		case GLFW_KEY_RIGHT:
			pressed_key = RIGHT_KEY;
			break;
		case GLFW_KEY_LEFT:
			pressed_key = LEFT_KEY;
			break;

		sInputLayer *input = get_game_input_instance();
		input->keyboard[pressed_key] = (action == GLFW_PRESS) ? KEY_PRESSED : KEY_RELEASED;
	};


}

void mouse_button_callback(GLFWwindow *wind, int button, int action, int mods) {
	char index;

	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		index = LEFT_CLICK;
		break;

	case GLFW_MOUSE_BUTTON_RIGHT:
		index = RIGHT_CLICK;
		break;

	case GLFW_MOUSE_BUTTON_MIDDLE:
		index = MIDDLE_CLICK;
		break;
	}

	sInputLayer *input = get_game_input_instance();
	input->mouse_clicks[index] = (action == GLFW_PRESS) ? KEY_PRESSED : KEY_RELEASED;
}

void cursor_enter_callback(GLFWwindow *window, int entered) {
	sInputLayer *input = get_game_input_instance();
	input->is_mouse_on_screen = entered;
}

void draw_loop(GLFWwindow *window) {
	glfwMakeContextCurrent(window);

	// Config scene
	sCamera camera;
	glm::vec3 camera_original_position = glm::vec3{2.0f, 2.60f, 2.0f};
	camera.position = camera_original_position;
	camera.look_at(glm::vec3{0.0f, 0.0f, 0.0f});

	// Complex material cube
	sMeshRenderer cube_renderer;
	sMesh cube_mesh;
	cube_mesh.load_OBJ_mesh("resources/cube.obj");
	cube_renderer.create_from_mesh(&cube_mesh);

	sMaterial cube_material;
	cube_renderer.material.add_texture("resources/textures/normal.png", NORMAL_MAP);
	cube_renderer.material.add_texture("resources/textures/color.png", COLOR_MAP);
	cube_renderer.material.add_texture("resources/textures/rough.png", SPECULAR_MAP);
	cube_renderer.material.add_shader("resources/shaders/pbr.vs", "resources/shaders/pbr.fs");


	double prev_frame_time = glfwGetTime();
	sInputLayer *input_state = get_game_input_instance();

	glm::mat4x4 viewproj_mat = {};
	glm::mat4x4 proj_mat = {};

	glm::mat4x4 obj_model = {};
	//obj_model.set_identity();
	//obj_model.set_scale({1.0f, 1.0f, 1.f});

	float camera_angle = 274.001f;

	while(!glfwWindowShouldClose(window)) {
		// Draw loop
		int width, heigth;
		double temp_mouse_x, temp_mouse_y;
		
		glfwGetFramebufferSize(window, &width, &heigth);
		// Set to OpenGL viewport size anc coordinates
		glViewport(0,0, width, heigth);

		float aspect_ratio = (float) width / heigth;

		// OpenGL stuff
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
		glEnable(GL_DEPTH_TEST);

		ImGui_ImplOpenGL3_NewFrame();
    	ImGui_ImplGlfw_NewFrame();
    	ImGui::NewFrame();

		double curr_frame_time = glfwGetTime();
		double elapsed_time = curr_frame_time - prev_frame_time;

		// Mouse position control
		glfwGetCursorPos(window, &temp_mouse_x, &temp_mouse_y);
		input_state->mouse_speed_x = abs(input_state->mouse_pos_x - temp_mouse_x) * elapsed_time;
		input_state->mouse_speed_y = abs(input_state->mouse_pos_y - temp_mouse_y) * elapsed_time;
		input_state->mouse_pos_x = temp_mouse_x;
		input_state->mouse_pos_y = temp_mouse_y;

		// ImGui
		ImGui::Begin("Scene control");

		// Rotate the camera arround
		if (ImGui::SliderFloat("Camera rotation", &camera_angle, 0.01f, 360.0f)) {
			camera.position.x = (camera_original_position.x * cos(camera_angle / (180.0f / PI))) - (camera_original_position.z * sin(camera_angle/ (180.0f / PI)));
			camera.position.z = (camera_original_position.z * sin(camera_angle/ (180.0f / PI))) + (camera_original_position.x * cos(camera_angle/ (180.0f / PI)));
		}

		ImGui::SliderFloat("Camera up-down", &camera.position.y, -3.01f, 8.0f);

		camera.look_at({0.0f, 0.0f, 0.0f});
		camera.get_perspective_viewprojection_matrix(90.0f, 1000.0f, 0.01f, aspect_ratio, &viewproj_mat);

		cube_renderer.render(&obj_model, 1, viewproj_mat, false, camera);

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	
	
	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_NAME, NULL, NULL);

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorEnterCallback(window, cursor_enter_callback);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (!window) {
		std::cout << "Error, could not create window" << std::endl; 
	} else {
		if (!gl3wInit()) {
			// IMGUI version
			//IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO &io = ImGui::GetIO();
			// Platform IMGUI
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init("#version 130");
			ImGui::StyleColorsDark();
			draw_loop(window);
		} else {
			std::cout << "Cannot init gl3w" << std::endl;
		}
		
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
