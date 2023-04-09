#include "glfw_window.hpp"

#include <golxzn/core/types/time.hpp>
#include <golxzn/core/types/clock.hpp>
#include <golxzn/core/resources/manager.hpp>
#include <golxzn/render.hpp>
#include <golxzn/graphics/types/shader/program.hpp>
#include <golxzn/graphics/types/texture/texture.hpp>
#include <golxzn/graphics/types/model/material.hpp>
#include <golxzn/graphics/types/model/mesh.hpp>
#include <golxzn/graphics/types/model/presets.hpp>

#include <golxzn/graphics/controller/opengl/VAO.hpp>
#include <golxzn/graphics/controller/opengl/EBO.hpp>

#include "teapot.hpp"

int main() {
	using namespace golxzn;
	using namespace types_literals;
	using graphics::types::vertex;

	static constexpr auto FPS{ 75_f16 };
	static constexpr auto frame_time{ 1.0_f16 / FPS };

	spdlog::set_level(spdlog::level::debug);

	core::res_man::initialize("opengl_mesh");

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
			std::make_pair("u_texture", graphics::types::texture::make(graphics::types::tex_type::cube_map, "res://textures/cube_maps/skybox.jpg")),
		}
	};
	cube_map_mesh.get_mod<graphics::mods::mod_capabilities>()
		->reset(graphics::mods::capabilities::depth_test);

	auto diffuse0{ graphics::types::texture::make(
		graphics::types::texture::type::texture_2d, "res://textures/moaning_pink.jpg") };
	if (diffuse0 && diffuse0->valid()) {
		diffuse0->generate_mip_maps();
	}

	auto plane_preset{ graphics::presets::plane_vertices(white) };
	graphics::types::mesh plane{ "plane",
		std::move(plane_preset.vertices), std::move(plane_preset.indices),
		graphics::types::shader_program::make("texture", {
			"res://shaders/mesh.vert",
			"res://shaders/mesh.frag",
		}),
		graphics::types::material::make("plane_material",
			glm::vec3{ 0.0_f16, 0.0_f16, 0.0_f16 }, // ambient
			glm::vec3{ 1.0_f16, 1.0_f16, 1.0_f16 }, // diffuse
			glm::vec3{ 0.1_f16, 0.1_f16, 0.1_f16 }, // specular
			32.0_f16 // shininess
		),
		{
			std::make_pair("u_diffuse0", std::move(diffuse0)),
		}
	};

	graphics::types::mesh teapot_mesh{ "teapot",
		teapot_verices, teapot_triangles,
		graphics::types::shader_program::make("mesh_sp", {
			"res://shaders/mesh.vert",
			"res://shaders/mesh.frag",
		}),
		graphics::types::material::make("teapot_material",
			glm::vec3{ 0.2_f16, 0.1_f16, 0.5_f16 }, // ambient
			glm::vec3{ 0.6_f16, 0.1_f16, 0.9_f16 }, // diffuse
			glm::vec3{ 0.5_f16, 0.5_f16, 0.5_f16 }, // specular
			32.0_f16 // shininess
		)
	};

	static constexpr glm::vec3 up{ 0.0_f16, 1.0_f16, 0.0_f16 };
	struct light {
		glm::vec3 position{ 1.0_f32, 1.0_f32, -1.0_f32 };
		glm::vec3 color{ 1.0_f32, 1.0_f32, 1.0_f32 };
		glm::vec3 specular{ 1.0_f32, 1.0_f32, 1.0_f32 };
	};
	static constexpr light u_light;

	glm::mat4 projection{ glm::perspective(
		glm::radians(35.0_f16),
		window::aspect,
		0.001_f16,
		1000.0_f16
	) };

	glm::mat4 model{ 1.0_f16 };
	const glm::mat4 plane_transform{
		glm::translate(
			glm::rotate(glm::mat4{ 1.0_f16 }, 1.5708_f16, glm::vec3{ 1.0_f16, 0.0_f16, 0.0_f16 }),
			glm::vec3{  0.0_f16, 0.0_f16, 0.05_f16 }
		),
	};

	glm::vec3 camera_pos{ 0.0_f16, 2.0_f16, 4.0_f16 };
	glm::mat4 view{ glm::lookAt(
		camera_pos,
		glm::vec3(model[3]),
		glm::vec3(0.0_f16, 1.0_f16, 0.0_f16)
	) };

	core::clock clock;

	// render loop
	// -----------
	core::f32 time{};
	glClearColor(0.999_f16, 0.666_f16, 0.777_f16, 1.0_f16);
	while (!glfwWindowShouldClose(window)) {
		const auto delta{ clock.elapsed().seconds() };

		if (delta < frame_time) {
			glfwPollEvents();
			continue;
		}
		clock.restart();
		time += delta;

		// model = glm::rotate_slow(model, glm::radians(1.0_f16), up);
		const glm::mat3 rotator{
			glm::rotate(glm::mat4{ 1.0_f16 }, glm::radians(30.0_f16) * static_cast<core::f16>(delta), up)
		};

		camera_pos.y = glm::sin(time) * 2.0_f16;
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

		// Has to be first, before all other rendering (except for the cube map)
		golxzn::render::gizmos::draw_infinity_grid(view, projection);

		if (auto plane_shader{ plane.get_shader_program() }; plane_shader != nullptr) {
			plane_shader->use();
			plane_shader->set_uniform("projection", projection);
			plane_shader->set_uniform("view", view);
			plane_shader->set_uniform("model", plane_transform);
			plane_shader->set_uniform("u_light.position", u_light.position);
			plane_shader->set_uniform("u_light.color", u_light.color);
			plane_shader->set_uniform("u_light.specular", u_light.specular);
			plane_shader->unuse();
		}
		plane.draw();

		if (auto teapot_mesh_shader{ teapot_mesh.get_shader_program() }; teapot_mesh_shader != nullptr) {
			teapot_mesh_shader->use();
			teapot_mesh_shader->set_uniform("projection", projection);
			teapot_mesh_shader->set_uniform("view", view);
			teapot_mesh_shader->set_uniform("model", model);
			teapot_mesh_shader->set_uniform("u_light.position", u_light.position);
			teapot_mesh_shader->set_uniform("u_light.color", u_light.color);
			teapot_mesh_shader->set_uniform("u_light.specular", u_light.specular);
			teapot_mesh_shader->unuse();
		}
		teapot_mesh.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
}
