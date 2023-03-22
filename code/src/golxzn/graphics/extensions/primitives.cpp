#include "golxzn/common.hpp"
#include <golxzn/core/constants.hpp>

#include "golxzn/graphics/extensions/primitives.hpp"

#include "golxzn/graphics/types/model/mesh.hpp"
#include "golxzn/graphics/controller/controller.hpp"


namespace golxzn::graphics::extensions {

core::umap<std::string, types::shader_program::ref> primitives::mCachedShaders{};
core::umap<std::string, types::mesh::ref> primitives::mCachedMeshes{};

void primitives::draw_line(const glm::vec2 &start, const glm::vec2 &end, const core::f16 width, const glm::vec4 &color) {
	if (width < 0.0f) {
		spdlog::error("[{}] Width must be positive, but it equals {}!", class_name, width);
		return;
	}

	auto mesh{ get_line_mesh(default_circle_steps) };
	if (mesh == nullptr || !mesh->valid()) {
		spdlog::error("[{}] Failed to load line mesh!", class_name);
		return;
	}

	auto shader{ mesh->get_shader_program() };
	if (shader == nullptr || !shader->valid()) {
		spdlog::error("[{}] Failed to load line shader!", class_name);
		return;
	}

	const auto dir{ glm::normalize(end - start) };
	const core::f16 angle{ glm::acos(glm::dot(glm::vec2{ 1.0f, 0.0f }, dir)) };

	auto model = glm::translate(glm::mat4{ 1.0f }, glm::vec3{ start, 0.0f });
	model = glm::rotate(model, -angle, glm::vec3{ 0.0f, 0.0f, 1.0f });
	model = glm::scale(model, glm::vec3{ glm::length(end - start), width, 0.0f });

	// const auto view_matrix{ controller::get()->get_active_camera()->get_view_matrix() };
	// const auto projection_matrix{ controller::get()->get_active_camera()->get_projection_matrix() };
	const auto mvp_matrix{ /*projection_matrix * view_matrix * */ model };

	shader->use();
	shader->set_uniform("u_mvp_matrix", mvp_matrix);
	shader->set_uniform("u_color", color);
	shader->unuse();
	mesh->draw();

}

void primitives::draw_rectangle(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) {
	auto mesh{ get_rect_mesh() };
	if (mesh == nullptr || !mesh->valid()) {
		spdlog::error("[{}] Failed to load rectangle mesh!", class_name);
		return;
	}

	auto shader{ mesh->get_shader_program() };
	if (shader == nullptr || !shader->valid()) {
		spdlog::error("[{}] Failed to load rectangle shader!", class_name);
		return;
	}

	auto model = glm::translate(glm::mat4{ 1.0f }, glm::vec3{ position, 0.0f });
	model = glm::scale(model, glm::vec3{ size, 0.0f });

	// const auto view_matrix{ controller::get()->get_active_camera()->get_view_matrix() };
	// const auto projection_matrix{ controller::get()->get_active_camera()->get_projection_matrix() };
	const auto mvp_matrix{ /*projection_matrix * view_matrix * */ model };

	shader->use();
	shader->set_uniform("u_mvp_matrix", mvp_matrix);
	shader->set_uniform("u_color", color);
	shader->unuse();
	mesh->draw();

}

void primitives::draw_circle(const glm::vec2 &center, const core::f16 radius, const glm::vec4 &color, const core::u32 steps) {
	if (color.a <= 0.0f) return;
	if (radius < 0.0f) {
		spdlog::error("[{}] Radius must be positive, but it equals {}!", class_name, radius);
		return;
	}

	auto mesh{ get_circle_mesh(steps) };
	if (mesh == nullptr || !mesh->valid()) {
		spdlog::error("[{}] Failed to load circle mesh!", class_name);
		return;
	}

	auto shader{ mesh->get_shader_program() };
	if (shader == nullptr || !shader->valid()) {
		spdlog::error("[{}] Failed to load circle shader!", class_name);
		return;
	}

	const glm::vec3 model{ center, 0.0f };
	const glm::vec3 scale{ radius, radius, 1.0f };

	const auto model_matrix{ glm::scale(glm::translate(glm::mat4{ 1.0f }, model), scale) };
	// const auto view_matrix{ controller::get()->get_active_camera()->get_view_matrix() };
	// const auto projection_matrix{ controller::get()->get_active_camera()->get_projection_matrix() };
	const auto mvp_matrix{ /*projection_matrix * view_matrix * */ model_matrix };

	shader->use();
	shader->set_uniform("u_mvp_matrix", mvp_matrix);
	shader->set_uniform("u_color", color);
	shader->unuse();
	mesh->draw();
}

types::mesh::ref primitives::get_line_mesh(const core::u32 steps) {
	static const std::string name{ "line_2d" };
	if (auto found{ mCachedMeshes.find(name) }; found != std::end(mCachedMeshes)) {
		return found->second;
	}
	spdlog::info("[{}] Loading line mesh", class_name);

	static constexpr core::f16 width{ 1.00f };
	static constexpr glm::vec3 color{ 1.0f, 1.0f, 1.0f };
	static constexpr glm::vec2 uv{};

	auto line{ types::mesh::make(name,
		{
			types::vertex{ glm::vec3{ 0.0f,  width, 0.0f }, color, uv },
			types::vertex{ glm::vec3{ 1.0f,  width, 0.0f }, color, uv },
			types::vertex{ glm::vec3{ 1.0f, -width, 0.0f }, color, uv },
			types::vertex{ glm::vec3{ 0.0f, -width, 0.0f }, color, uv },
		},
		{ 0, 1, 2, 0, 2, 3 },
		get_default_2d_shader()
	) };
	if (line != nullptr) {
		line->get_mod<mods::mod_capabilities>()->reset(mods::capabilities::depth_test);
		mCachedMeshes.emplace(name, line);
	}
	return line;
}

types::mesh::ref primitives::get_rect_mesh() {
	static const std::string name{ "rectangle_2d" };
	if (auto found{ mCachedMeshes.find(name) }; found != std::end(mCachedMeshes)) {
		return found->second;
	}
	spdlog::info("[{}] Loading line mesh", class_name);

	static constexpr glm::vec3 color{ 1.0f, 1.0f, 1.0f };
	static constexpr glm::vec2 uv{};

	auto rect{ types::mesh::make(name,
		{
			types::vertex{ glm::vec3{ 0.0f, 1.0f, 0.0f }, color, uv },
			types::vertex{ glm::vec3{ 1.0f, 1.0f, 0.0f }, color, uv },
			types::vertex{ glm::vec3{ 1.0f, 0.0f, 0.0f }, color, uv },
			types::vertex{ glm::vec3{ 0.0f, 0.0f, 0.0f }, color, uv },
		},
		{ 0, 1, 2, 0, 2, 3 },
		get_default_2d_shader()
	) };
	if (rect != nullptr) {
		rect->get_mod<mods::mod_capabilities>()->reset(mods::capabilities::depth_test);
		mCachedMeshes.emplace(name, rect);
	}
	return rect;
}

types::mesh::ref primitives::get_circle_mesh(const core::u32 steps) {
	static const std::string name{ "circle_2d" };
	if (auto found{ mCachedMeshes.find(name) }; found != std::end(mCachedMeshes)) {
		return found->second;
	}

	spdlog::info("[{}] Loading circle mesh", class_name);
	static constexpr core::f16 pi2{ core::constants::pi_v<core::f16> * 2.0f };
	static constexpr glm::vec3 color{ 1.0f, 1.0f, 1.0f };
	static constexpr glm::vec2 uv{};

	std::vector<types::vertex> vertices;
	vertices.reserve(steps + 1);

	std::vector<core::u32> indices;
	indices.reserve((steps + 1) * 3);

	const auto angle_coef{ pi2 / static_cast<core::f16>(steps) };
	for (core::u32 i{}; i < steps; ++i) {
		const auto angle{ angle_coef * static_cast<core::f16>(i) };
		vertices.emplace_back(types::vertex{
			glm::vec3{ glm::sin(angle), glm::cos(angle), 0.0f }, color, uv
		});

		indices.emplace_back(i);
		indices.emplace_back((i + 1) % steps);
		indices.emplace_back(steps);
	}
	vertices.emplace_back(types::vertex{ glm::vec3{ 0.0f }, color, uv });

	auto circle{ types::mesh::make(name, vertices, indices, get_default_2d_shader()) };
	if (circle != nullptr) {
		circle->get_mod<mods::mod_capabilities>()->reset(mods::capabilities::depth_test);
		mCachedMeshes.emplace(name, circle);
	}
	return circle;
}

types::shader_program::ref primitives::load_shader(const std::string &name) {
	if (name.empty()) return nullptr;

	if (auto found{ mCachedShaders.find(name) }; found != mCachedShaders.end()) {
		return found->second;
	}

	const std::string path{ "res://shaders/primitives/" + name };
	auto program{ types::shader_program::make(name, {
		path + ".vert",
		path + ".frag"
	}) };

	if (program && program->get_status() == program_status::link_success) {
		mCachedShaders.emplace(name, program);
		return program;
	}

	spdlog::error("[{}] Failed to load shader program: {}!", class_name, name);
	return nullptr;
}

types::shader_program::ref primitives::get_default_2d_shader() {
	static const std::string name{ "default_2d" };
	if (auto found{ mCachedShaders.find(name) }; found != mCachedShaders.end()) {
		return found->second;
	}

	auto program{ types::shader_program::make(name, {
		types::shader::make(
			name + "_vertex_shader",
			R"glsl(#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 uv;
out vec4 v_color;
uniform vec4 u_color;
uniform mat4 u_mvp_matrix;
void main() {
	v_color = u_color;
	gl_Position = u_mvp_matrix * vec4(position, 1.0);
}
)glsl",
			types::shader::type::vertex
		),
		types::shader::make(
			name + "_fragment_shader",
			R"glsl(#version 330 core
in vec4 v_color;
out vec4 fragment_color;
void main() {
	vec3 gamma_correction = pow(v_color.rgb, vec3(1.0 / 2.2));
	fragment_color = vec4(v_color.rgb, v_color.a);
}
)glsl",
			types::shader::type::fragment
		),
	}) };

	if (program && program->get_status() == program_status::link_success) {
		mCachedShaders.emplace(name, program);
		return program;
	}

	spdlog::error("[{}] Failed to load shader program: {}!", class_name, name);
	return nullptr;
}

} // namespace golxzn::graphics::extensions
