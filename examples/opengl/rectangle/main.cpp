#include <golxzn/common.hpp>

#include <golxzn/core/resources/manager.hpp>
#include <golxzn/render.hpp>
#include <golxzn/graphics/window/window.hpp>
#include <golxzn/graphics/engines/opengl/VAO.hpp>
#include <golxzn/graphics/engines/opengl/EBO.hpp>
#include <golxzn/graphics/types/shader_program.hpp>

#include <iostream>

void processInput(GLFWwindow *window);


int main() {
	using namespace golxzn;
	using namespace types_literals;
	core::res_man::initialize("opengl_triangle");
	graphics::controller::initialize(graphics::controller::api_type::opengl);

	auto program{ graphics::types::shader_program::make("default", {
		"res://shaders/default.vs.glsl",
		"res://shaders/default.fs.glsl",
	}) };
	if (program->get_status() == graphics::program_status::need_to_link) {
		program->link();
	}

	graphics::gl::VAO VAO;

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	VAO.bind();
	graphics::gl::VBO VBO{
		// X     Y     Z      R     G     B
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,  0.9f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  1.0f, 0.5f, 0.2f,   // bottom left
		-0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 0.9f,   // top left
	};
	graphics::gl::EBO EBO{  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	VAO.link_attribute(VBO, 0, 3, GL_FLOAT, 6 * sizeof(core::f16), (void*)0);
	VAO.link_attribute(VBO, 1, 3, GL_FLOAT, 6 * sizeof(core::f16), (void*)(3 * sizeof(core::f16)));

	VBO.unbind();
	VAO.unbind();


	// uncomment this call to draw in wireframe polygons.
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	auto window{ graphics::window::api() };
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
		program->use();
		VAO.bind(); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// glBindVertexArray(0); // no need to unbind it every time
		program->unuse();

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
	program->clear();

	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
