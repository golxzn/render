#include <golxzn/common.hpp>

#include <golxzn/core/resources/manager.hpp>
#include <golxzn/render.hpp>
#include <golxzn/graphics/window/window.hpp>
#include <golxzn/graphics/engines/opengl/VAO.hpp>
#include <golxzn/graphics/engines/opengl/EBO.hpp>
#include <golxzn/graphics/types/shader_program.hpp>
#include <golxzn/graphics/types/texture.hpp>

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
		"res://shaders/texture.vs.glsl",
		"res://shaders/texture.fs.glsl",
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
		"res://shaders/cube_map.vs.glsl",
		"res://shaders/cube_map.fs.glsl",
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

	graphics::gl::VAO VAO;

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	VAO.bind();
	graphics::gl::VBO VBO{
		//  X         Y        Z         R        G        B          U         V
		 0.5_f16,  0.5_f16, 0.0_f16,  1.0_f16, 1.0_f16, 1.0_f16,   2.0_f16,  1.5_f16,   // top right
		 0.5_f16, -0.5_f16, 0.0_f16,  0.9_f16, 0.0_f16, 0.0_f16,   2.0_f16, -0.5_f16,   // bottom right
		-0.5_f16, -0.5_f16, 0.0_f16,  1.0_f16, 0.5_f16, 0.2_f16,   0.0_f16, -0.5_f16,   // bottom left
		-0.5_f16,  0.5_f16, 0.0_f16,  0.0_f16, 0.0_f16, 0.9_f16,   0.0_f16,  1.5_f16,   // top left
	};
	graphics::gl::EBO EBO{  // note that we start from 0!
		0_u32, 1_u32, 3_u32,  // first Triangle
		1_u32, 2_u32, 3_u32   // second Triangle
	};

	static constexpr auto stride{ 8 * sizeof(core::f16) };

	VAO.link_attribute(VBO, 0, 3, GL_FLOAT, stride, (void*)0);
	VAO.link_attribute(VBO, 1, 3, GL_FLOAT, stride, (void*)(3 * sizeof(core::f16)));
	VAO.link_attribute(VBO, 2, 2, GL_FLOAT, stride, (void*)(6 * sizeof(core::f16)));

	VBO.unbind();
	VAO.unbind();

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



		diffuse0->bind(1);
		diffuse1->bind(0);
		program->use();

		view = glm::lookAt(
			camera_pos,
			glm::vec3(0.0_f16, 0.0_f16, 0.0_f16),
			glm::vec3(0.0_f16, 1.0_f16, 0.0_f16)
		);
		program->set_uniform("projection", projection);
		program->set_uniform("view", view);
		program->set_uniform("model", model);

		VAO.bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		VAO.unbind();

		program->unuse();
		diffuse1->unbind();
		diffuse0->unbind();


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
}
