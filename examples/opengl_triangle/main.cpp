#include <golxzn/common.hpp>

#include <golxzn/core/resources/manager.hpp>
#include <golxzn/render.hpp>
#include <golxzn/graphics/window/window.hpp>
#include <golxzn/graphics/engines/opengl/VAO.hpp>
#include <golxzn/graphics/engines/opengl/EBO.hpp>

#include <iostream>

using namespace golxzn;

void processInput(GLFWwindow *window);


int main() {
	golxzn::core::res_man::initialize("opengl_triangle");
	golxzn::graphics::controller::initialize(golxzn::graphics::controller::api_type::opengl);

	const auto vs_code{ golxzn::core::res_man::load_string("res://shaders/default.vs.glsl") };
	spdlog::info("Loaded vertex shader code size: {}", vs_code.size());
	const auto fs_code{ golxzn::core::res_man::load_string("res://shaders/default.fs.glsl") };
	spdlog::info("Loaded fragment shader code size: {}", fs_code.size());

	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	const auto *const vs_code_cstr{ vs_code.c_str() };
	glShaderSource(vertexShader, 1, &vs_code_cstr, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		spdlog::error("[ERROR::SHADER::VERTEX::COMPILATION_FAILED]:\n{}", infoLog);
	}
	// fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const auto *const fs_code_cstr{ fs_code.c_str() };
	glShaderSource(fragmentShader, 1, &fs_code_cstr, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		spdlog::error("[ERROR::SHADER::FRAGMENT::COMPILATION_FAILED]:\n{}", infoLog);
	}
	// link shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		spdlog::error("[ERROR::SHADER::PROGRAM::LINKING_FAILED]:\n{}", infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------



	graphics::gl::VAO VAO;

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	VAO.bind();
	graphics::gl::VBO VBO{ std::vector<core::f16>{
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left
	} };
	graphics::gl::EBO EBO{ std::vector<core::u32> {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	} };

	VAO.link_attribute(VBO, 0, 3, GL_FLOAT, 3 * sizeof(core::f16), (void*)0);

	VBO.unbind();
	VAO.unbind();


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	auto window{ golxzn::graphics::window::api() };
	if (window == nullptr) {
		spdlog::critical("No window API");
		return -1;
	}

	// render loop
	// -----------
	while (!window->should_close()) {

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		glUseProgram(shaderProgram);
		VAO.bind(); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// glBindVertexArray(0); // no need to unbind it every time

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		window->swap_buffers();
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	VAO.clean();
	VBO.clean();
	EBO.clean();
	glDeleteProgram(shaderProgram);

	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
