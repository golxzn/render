#include <golxzn/common.hpp>

#include <golxzn/core/resources/manager.hpp>
#include <golxzn/render.hpp>
#include <golxzn/graphics/window/window.hpp>
#include <golxzn/graphics/types/shader_program.hpp>
#include <golxzn/graphics/types/texture.hpp>
#include <golxzn/graphics/types/material.hpp>
#include <golxzn/graphics/types/mesh.hpp>

#include <golxzn/graphics/controller/opengl/VAO.hpp>
#include <golxzn/graphics/controller/opengl/EBO.hpp>

#include "teapot.hpp"

int main() {
	using namespace golxzn;
	using namespace types_literals;
	using graphics::types::vertex;

	glfwSetErrorCallback([](int code, const char *desc) {
		spdlog::error("Error [{}]: {}", code, desc);
	});

	spdlog::set_level(spdlog::level::debug);
	core::res_man::initialize("opengl_triangle");
	graphics::controller::initialize(graphics::controller::api_type::opengl);

	auto window{ graphics::window::api() };
	if (window == nullptr) {
		spdlog::critical("No window API");
		return -1;
	}

	static constexpr glm::vec3 white{ 1.0_f16, 1.0_f16, 1.0_f16 };
	graphics::types::mesh cube_map_mesh{ "cube_map",
		std::vector<vertex>{
			{ glm::vec3{ -1.0_f16,  1.0_f16, -1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{ -1.0_f16, -1.0_f16, -1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{  1.0_f16, -1.0_f16, -1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{  1.0_f16, -1.0_f16, -1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{  1.0_f16,  1.0_f16, -1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{ -1.0_f16,  1.0_f16, -1.0_f16 }, white, glm::vec2{} },

			{ glm::vec3{ -1.0_f16, -1.0_f16,  1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{ -1.0_f16, -1.0_f16, -1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{ -1.0_f16,  1.0_f16, -1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{ -1.0_f16,  1.0_f16, -1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{ -1.0_f16,  1.0_f16,  1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{ -1.0_f16, -1.0_f16,  1.0_f16 }, white, glm::vec2{} },

			{ glm::vec3{  1.0_f16, -1.0_f16, -1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{  1.0_f16, -1.0_f16,  1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{  1.0_f16,  1.0_f16,  1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{  1.0_f16,  1.0_f16,  1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{  1.0_f16,  1.0_f16, -1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{  1.0_f16, -1.0_f16, -1.0_f16 }, white, glm::vec2{} },

			{ glm::vec3{ -1.0_f16, -1.0_f16,  1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{ -1.0_f16,  1.0_f16,  1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{  1.0_f16,  1.0_f16,  1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{  1.0_f16,  1.0_f16,  1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{  1.0_f16, -1.0_f16,  1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{ -1.0_f16, -1.0_f16,  1.0_f16 }, white, glm::vec2{} },

			{ glm::vec3{ -1.0_f16,  1.0_f16, -1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{  1.0_f16,  1.0_f16, -1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{  1.0_f16,  1.0_f16,  1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{  1.0_f16,  1.0_f16,  1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{ -1.0_f16,  1.0_f16,  1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{ -1.0_f16,  1.0_f16, -1.0_f16 }, white, glm::vec2{} },

			{ glm::vec3{ -1.0_f16, -1.0_f16, -1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{ -1.0_f16, -1.0_f16,  1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{  1.0_f16, -1.0_f16, -1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{  1.0_f16, -1.0_f16, -1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{ -1.0_f16, -1.0_f16,  1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{  1.0_f16, -1.0_f16,  1.0_f16 }, white, glm::vec2{} },
		}, {},
		graphics::types::shader_program::make("cube_map_shader", {
			"res://shaders/cube_map.vert",
			"res://shaders/cube_map.frag",
		}),
		nullptr,
		{
			std::make_pair("skybox", graphics::types::texture::make(graphics::types::texture::type::cube_map, "res://textures/cube_maps/skybox.jpg")),
		}
	};
	cube_map_mesh.depth_test(false);

	auto diffuse0{ graphics::types::texture::make(
		graphics::types::texture::type::texture_2d, "res://textures/moaning_pink.jpg") };
	if (diffuse0->valid()) {
		using namespace graphics::types;
		diffuse0->generate_mip_maps();
		auto wrap{ diffuse0->param<texture::wrap>() };
		wrap[texture::wrap::type::s] = texture::wrap::mode::mirrored_repeat;
		wrap[texture::wrap::type::t] = texture::wrap::mode::clamp_to_edge;
	}

	spdlog::info("Creating mesh teapot");
	graphics::types::mesh teapot_mesh{ "teapot",
		teapot_verices,
		teapot_triangles,
		graphics::types::shader_program::make("mesh_sp", {
			"res://shaders/mesh.vert",
			"res://shaders/mesh.frag",
		}),
		graphics::types::material::make("teapot_material",
			glm::vec3{ 1.0_f16, 0.5_f16, 0.3_f16 }, // ambient
			glm::vec3{ 1.0_f16, 0.5_f16, 0.3_f16 }, // diffuse
			glm::vec3{ 0.5_f16, 0.5_f16, 0.5_f16 }, // specular
			32.0_f16 // shininess
		),
		{} // no textures
	};

	static constexpr glm::vec3 up{ 0.0_f16, 1.0_f16, 0.0_f16 };

	using win_impl = graphics::window::implementation;

	glm::mat4 projection{ glm::perspective(
		glm::radians(35.0_f16),
		static_cast<float>(win_impl::default_width) / static_cast<float>(win_impl::default_height),
		0.001_f16,
		1000.0_f16
	) };

	glm::mat4 model{ 1.0_f16 };

	glm::vec3 camera_pos{ 0.0_f16, 0.0_f16, 3.0_f16 };
	glm::mat4 view{ glm::lookAt(
		camera_pos,
		glm::vec3(model[3]),
		glm::vec3(0.0_f16, 1.0_f16, 0.0_f16)
	) };

	// uncomment this call to draw in wireframe polygons.
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	core::f32 current_frame{ glfwGetTime() };
	core::f32 last_frame{};
	core::f32 delta{};

	// render loop
	// -----------
	glClearColor(0.999_f16, 0.666_f16, 0.777_f16, 1.0_f16);
	while (!window->should_close()) {

		current_frame = glfwGetTime();
		delta = current_frame - last_frame;
		last_frame = current_frame;

		// model = glm::rotate_slow(model, glm::radians(1.0_f16), up);
		const glm::mat3 rotator{
			glm::rotate(glm::mat4{ 1.0_f16 }, glm::radians(30.0_f16) * static_cast<core::f16>(delta), up)
		};

		camera_pos = rotator * camera_pos;

		// render
		// ------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view = glm::mat4(glm::mat3(view));
		auto cube_map_shader{ cube_map_mesh.get_shader_program() };
		cube_map_shader->use();
		cube_map_shader->set_uniform("projection", projection);
		cube_map_shader->set_uniform("view", view);
		cube_map_shader->unuse();

		cube_map_mesh.draw();

		view = glm::lookAt(
			camera_pos,
			glm::vec3(0.0_f16, 0.0_f16, 0.0_f16),
			glm::vec3(0.0_f16, 1.0_f16, 0.0_f16)
		);

		auto teapot_mesh_shader{ teapot_mesh.get_shader_program() };
		teapot_mesh_shader->use();
		teapot_mesh_shader->set_uniform("projection", projection);
		teapot_mesh_shader->set_uniform("view", view);
		teapot_mesh_shader->set_uniform("model", model);
		teapot_mesh_shader->unuse();
		teapot_mesh.draw();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		window->swap_buffers();
		glfwPollEvents();
	}

	// cube_map_vbo.clean();
	// cube_map_vao.clean();
}