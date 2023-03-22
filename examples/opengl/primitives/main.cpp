#include "glfw_window.hpp"

#include <golxzn/core/resources/manager.hpp>
#include <golxzn/render.hpp>

int main() {
	using namespace golxzn;
	using namespace types_literals;

	glfwSetErrorCallback([](int code, const char *desc) {
		spdlog::error("[glfw] [{}] {}", code, desc);
	});

	spdlog::set_level(spdlog::level::debug);

	core::res_man::initialize("opengl_primitives");

	auto window{ window::initialize("golxzn | mesh") };
	if (window == nullptr) return -1;

	if (!graphics::controller::initialize(graphics::controller::api_type::opengl,
			(graphics::controller::get_process_address_function)glfwGetProcAddress)) {
		return -1;
	}

	glfwSetWindowSize(window, 800, 800);

	// uncomment this call to draw in wireframe polygons.
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	core::f32 current_frame{ glfwGetTime() };
	core::f32 last_frame{};
	core::f32 delta{};

	// render loop
	// -----------
	glClearColor(0.999_f16, 0.666_f16, 0.777_f16, 1.0_f16);
	while (!glfwWindowShouldClose(window)) {

		current_frame = glfwGetTime();
		delta = current_frame - last_frame;
		last_frame = current_frame;


		// // render
		// // ------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		golxzn::render::draw_rectangle({ -0.25_f16, -0.25_f16 }, { 0.5_f16, 0.5_f16 }, { 0.0f, 0.0f, 0.0f, 0.5f });

		golxzn::render::draw_line({  0.0_f16,  0.75_f16 }, {  0.0_f16,  -0.75_f16 }, 0.01_f16);
		golxzn::render::draw_line({ -0.75_f16, 0.0_f16  }, {  0.75_f16,  0.0_f16  }, 0.01_f16, { 1.0f, 0.0f, 0.0f, 1.0f });
		golxzn::render::draw_line({  0.5_f16,  0.5_f16  }, { -0.5_f16,  -0.5_f16  }, 0.01_f16, { 0.0f, 0.0f, 1.0f, 1.0f });

		golxzn::render::draw_circle({ -0.5_f16, -0.5_f16 }, 0.2_f16, { 1.0f, 0.0f, 0.0f, 1.0f });
		golxzn::render::draw_circle({  0.5_f16, -0.5_f16 }, 0.2_f16, { 0.0f, 1.0f, 0.0f, 1.0f });
		golxzn::render::draw_circle({ -0.5_f16,  0.5_f16 }, 0.2_f16, { 0.0f, 0.0f, 1.0f, 1.0f });
		golxzn::render::draw_circle({  0.5_f16,  0.5_f16 }, 0.2_f16, { 0.3f, 0.3f, 0.3f, 1.0f });

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	spdlog::info("[main] exit");
}
