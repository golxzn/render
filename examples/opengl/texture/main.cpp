#include "glfw_window.hpp"

#include <golxzn/core/types/time.hpp>
#include <golxzn/core/types/clock.hpp>
#include <golxzn/core/resources/manager.hpp>
#include <golxzn/render.hpp>
#include <golxzn/graphics/types/shader/program.hpp>
#include <golxzn/graphics/types/texture/texture.hpp>
#include <golxzn/graphics/types/model/mesh.hpp>
#include <golxzn/graphics/types/model/material.hpp>
#include <golxzn/graphics/types/model/presets.hpp>

int main() {
	using namespace golxzn;
	using namespace types_literals;
	using graphics::types::vertex;

	glfwSetErrorCallback([](int code, const char *desc) {
		spdlog::error("Error [{}]: {}", code, desc);
	});

	spdlog::set_level(spdlog::level::debug);
	core::res_man::initialize("opengl_texture");

	auto window{ window::initialize("golxzn | mesh") };
	if (window == nullptr) return -1;

	if (!graphics::controller::initialize(graphics::controller::api_type::opengl,
			(graphics::controller::get_process_address_function)glfwGetProcAddress)) {
		return -1;
	}

	static constexpr glm::vec3 white{ 1.0_f16, 1.0_f16, 1.0_f16 };
	auto cube_map_preset{ graphics::presets::cube_map_vertices(white) };
	graphics::types::mesh cube_map_mesh{ "cube_map",
	 	std::move(cube_map_preset.vertices), std::move(cube_map_preset.indices),
		graphics::types::shader_program::make("cube_map_shader", {
			"res://shaders/cube_map.vert",
			"res://shaders/cube_map.frag",
		}),
		nullptr,
		{
			std::make_pair("skybox", graphics::types::texture::make(graphics::types::tex_type::cube_map, "res://textures/cube_maps/skybox.jpg")),
		}
	};
	cube_map_mesh.get_mod<graphics::mods::mod_capabilities>()
		->reset(graphics::mods::capabilities::depth_test);

	auto plane_preset{ graphics::presets::plane_vertices(white) };
	graphics::types::mesh plane{ "plane",
		std::move(plane_preset.vertices), std::move(plane_preset.indices),
		graphics::types::shader_program::make("default", {
			"res://shaders/texture.vert",
			"res://shaders/texture.frag",
		}), nullptr,
		{
			std::make_pair("diffuse0", graphics::types::texture::make(graphics::types::tex_type::texture_2d, "res://textures/moaning_pink.jpg")),
		}
	};

	static constexpr glm::vec3 up{ 0.0_f16, 1.0_f16, 0.0_f16 };

	glm::mat4 projection{ glm::perspective(
		glm::radians(35.0_f16),
		window::aspect,
		0.001_f16,
		1000.0_f16
	) };

	glm::vec3 camera_pos{ 0.0_f16, 0.0_f16, 6.0_f16 };
	glm::mat4 view{ glm::lookAt(
		camera_pos,
		glm::vec3(0.0_f16, 0.0_f16, 0.0_f16),
		glm::vec3(0.0_f16, 1.0_f16, 0.0_f16)
	) };
	glm::mat4 model{ 1.0_f16 };

	// uncomment this call to draw in wireframe polygons.
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	core::clock clock;

	// render loop
	// -----------
	glClearColor(0.999_f16, 0.666_f16, 0.777_f16, 1.0_f16);
	while (!glfwWindowShouldClose(window)) {

		const auto delta{ clock.restart().seconds() };

		// model = glm::rotate_slow(model, glm::radians(1.0_f16), up);
		const glm::mat3 rotator{
			glm::rotate(glm::mat4{ 1.0_f16 }, glm::radians(30.0_f16) * static_cast<core::f16>(delta), up)
		};

		camera_pos = rotator * camera_pos;

		// render
		// ------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		if (auto cube_map_shader{ cube_map_mesh.get_shader_program() }; cube_map_shader != nullptr) {
			cube_map_shader->use();
			cube_map_shader->set_uniform("projection", projection);
			cube_map_shader->set_uniform("view", glm::mat4(glm::mat3(view)));
			cube_map_shader->unuse();
		}
		cube_map_mesh.draw();


		view = glm::lookAt(
			camera_pos,
			glm::vec3(0.0_f16, 0.0_f16, 0.0_f16),
			glm::vec3(0.0_f16, 1.0_f16, 0.0_f16)
		);
		if (auto plane_shader{ plane.get_shader_program() }; plane_shader != nullptr) {
			plane_shader->use();
			plane_shader->set_uniform("projection", projection);
			plane_shader->set_uniform("view", view);
			plane_shader->set_uniform("model", glm::mat4{ 1.0_f16 });
			plane_shader->unuse();
		}
		plane.draw();


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
