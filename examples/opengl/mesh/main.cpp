#include <golxzn/common.hpp>

#include <golxzn/core/resources/manager.hpp>
#include <golxzn/render.hpp>
#include <golxzn/graphics/window/window.hpp>
#include <golxzn/graphics/types/shader_program.hpp>
#include <golxzn/graphics/types/texture.hpp>
#include <golxzn/graphics/types/mesh.hpp>

#include <golxzn/graphics/controller/opengl/VAO.hpp>
#include <golxzn/graphics/controller/opengl/EBO.hpp>

int main() {
	using namespace golxzn;
	using namespace types_literals;

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

	core::i32 max_texture_image_units{};
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_image_units);
	spdlog::info("Max texture image units {}", max_texture_image_units);

	auto program{ graphics::types::shader_program::make("default", {
		"res://shaders/mesh.vert",
		"res://shaders/mesh.frag",
	}) };
	if (program->get_status() == graphics::program_status::need_to_link) {
		program->link();
	}
	program->use();
	program->set_uniform("diffuse0", 0_i32);
	program->set_uniform("diffuse1", 1_i32);
	program->unuse();

	auto diffuse0{ graphics::types::texture::make(
		graphics::types::texture::type::texture_2d, "res://textures/moaning_pink.jpg") };
	if (diffuse0->valid()) {
		using namespace graphics::types;
		diffuse0->generate_mip_maps();
		auto wrap{ diffuse0->param<texture::wrap>() };
		wrap[texture::wrap::type::s] = texture::wrap::mode::mirrored_repeat;
		wrap[texture::wrap::type::t] = texture::wrap::mode::clamp_to_edge;
	}
	auto diffuse1{ graphics::types::texture::make(
		graphics::types::texture::type::texture_2d, "res://textures/cube_maps/skybox/back.jpg") };
	if (diffuse1->valid()) {
		using namespace graphics::types;
		diffuse1->generate_mip_maps();
		auto wrap{ diffuse1->param<texture::wrap>() };
		wrap[texture::wrap::type::s] = texture::wrap::mode::mirrored_repeat;
		wrap[texture::wrap::type::t] = texture::wrap::mode::clamp_to_edge;
	}



	auto cube_map_shader{ graphics::types::shader_program::make("cube_map", {
		"res://shaders/cube_map.vert",
		"res://shaders/cube_map.frag",
	}) };
	if (cube_map_shader->get_status() == graphics::program_status::need_to_link) {
		cube_map_shader->link();
	}

	auto cube_map{ graphics::types::texture::make(
		graphics::types::texture::type::cube_map, "res://textures/cube_maps/skybox.jpg") };
	if (cube_map->valid()) {
		cube_map->generate_mip_maps();
		cube_map_shader->use();
		cube_map_shader->set_uniform("skybox", 0_i32);
		// cube_map_shader->set_uniform<core::i32>("skybox", cube_map->id());
		cube_map_shader->unuse();
	}


	graphics::gl::VAO cube_map_vao;
	cube_map_vao.bind();
	graphics::gl::VBO cube_map_vbo{
		-1.0_f16,  1.0_f16, -1.0_f16,
		-1.0_f16, -1.0_f16, -1.0_f16,
		 1.0_f16, -1.0_f16, -1.0_f16,
		 1.0_f16, -1.0_f16, -1.0_f16,
		 1.0_f16,  1.0_f16, -1.0_f16,
		-1.0_f16,  1.0_f16, -1.0_f16,

		-1.0_f16, -1.0_f16,  1.0_f16,
		-1.0_f16, -1.0_f16, -1.0_f16,
		-1.0_f16,  1.0_f16, -1.0_f16,
		-1.0_f16,  1.0_f16, -1.0_f16,
		-1.0_f16,  1.0_f16,  1.0_f16,
		-1.0_f16, -1.0_f16,  1.0_f16,

		 1.0_f16, -1.0_f16, -1.0_f16,
		 1.0_f16, -1.0_f16,  1.0_f16,
		 1.0_f16,  1.0_f16,  1.0_f16,
		 1.0_f16,  1.0_f16,  1.0_f16,
		 1.0_f16,  1.0_f16, -1.0_f16,
		 1.0_f16, -1.0_f16, -1.0_f16,

		-1.0_f16, -1.0_f16,  1.0_f16,
		-1.0_f16,  1.0_f16,  1.0_f16,
		 1.0_f16,  1.0_f16,  1.0_f16,
		 1.0_f16,  1.0_f16,  1.0_f16,
		 1.0_f16, -1.0_f16,  1.0_f16,
		-1.0_f16, -1.0_f16,  1.0_f16,

		-1.0_f16,  1.0_f16, -1.0_f16,
		 1.0_f16,  1.0_f16, -1.0_f16,
		 1.0_f16,  1.0_f16,  1.0_f16,
		 1.0_f16,  1.0_f16,  1.0_f16,
		-1.0_f16,  1.0_f16,  1.0_f16,
		-1.0_f16,  1.0_f16, -1.0_f16,

		-1.0_f16, -1.0_f16, -1.0_f16,
		-1.0_f16, -1.0_f16,  1.0_f16,
		 1.0_f16, -1.0_f16, -1.0_f16,
		 1.0_f16, -1.0_f16, -1.0_f16,
		-1.0_f16, -1.0_f16,  1.0_f16,
		 1.0_f16, -1.0_f16,  1.0_f16
	};
	cube_map_vao.link_attribute(cube_map_vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void *)0);

	cube_map_vbo.unbind();
	cube_map_vao.unbind();

	spdlog::info("Creating mesh plane");
	using graphics::types::vertex;
	graphics::types::mesh plane_mesh{ "plane",
		std::vector<vertex>{ // Vertices
			{ glm::vec3{  0.5_f16,  0.5_f16, 0.0_f16 }, glm::vec3{ 1.f, 1.f, 1.f }, glm::vec2{ 2.0_f16,  1.5_f16 } },   // top right
			{ glm::vec3{  0.5_f16, -0.5_f16, 0.0_f16 }, glm::vec3{ 1.f, 1.f, 1.f }, glm::vec2{ 2.0_f16, -0.5_f16 } },   // bottom right
			{ glm::vec3{ -0.5_f16, -0.5_f16, 0.0_f16 }, glm::vec3{ 1.f, 1.f, 1.f }, glm::vec2{ 0.0_f16, -0.5_f16 } },   // bottom left
			{ glm::vec3{ -0.5_f16,  0.5_f16, 0.0_f16 }, glm::vec3{ 1.f, 1.f, 1.f }, glm::vec2{ 0.0_f16,  1.5_f16 } },   // top left
		},
		std::vector<core::u32>{ // Indices
			0_u32, 1_u32, 3_u32,  // first triangle
			1_u32, 2_u32, 3_u32   // second triangle
		},
		program,
		nullptr, // Material
		{ // Textures
			std::make_pair("diffuse0", diffuse0),
			std::make_pair("diffuse1", diffuse1),
		}
	};

	static constexpr glm::vec3 up{ 0.0_f16, 1.0_f16, 0.0_f16 };

	using win_impl = graphics::window::implementation;

	glm::mat4 projection{ glm::perspective(
		glm::radians(35.0_f16),
		static_cast<float>(win_impl::default_width) / static_cast<float>(win_impl::default_height),
		0.001_f16,
		1000.0_f16
	) };

	glm::vec3 camera_pos{ 0.0_f16, 0.0_f16, 3.0_f16 };
	glm::mat4 view{ glm::lookAt(
		camera_pos,
		glm::vec3(0.0_f16, 0.0_f16, 0.0_f16),
		glm::vec3(0.0_f16, 1.0_f16, 0.0_f16)
	) };
	glm::mat4 model{ 1.0_f16 };

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


		cube_map->bind();
		cube_map_shader->use();

		view = glm::mat4(glm::mat3(view));
		cube_map_shader->set_uniform("projection", projection);
		cube_map_shader->set_uniform("view", view);

		cube_map_vao.bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		cube_map_vao.unbind();

		cube_map_shader->unuse();
		cube_map->unbind();



		view = glm::lookAt(
			camera_pos,
			glm::vec3(0.0_f16, 0.0_f16, 0.0_f16),
			glm::vec3(0.0_f16, 1.0_f16, 0.0_f16)
		);
		program->use();
		program->set_uniform("projection", projection);
		program->set_uniform("view", view);
		program->set_uniform("model", model);
		program->unuse();

		plane_mesh.draw();


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		window->swap_buffers();
		glfwPollEvents();
	}

	cube_map_vbo.clean();
	cube_map_vao.clean();
	program->clear();
}
