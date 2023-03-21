#include <golxzn/common.hpp>
#include "golxzn/graphics/extensions/gizmos.hpp"

#include "golxzn/graphics/mods/capabilities.hpp"
#include "golxzn/graphics/mods/blend.hpp"
#include "golxzn/graphics/mods/depth.hpp"


namespace golxzn::graphics::extensions {

core::umap<std::string, types::shader_program::ref> ext_gizmos::gizmos::mCachedShaders{};
core::umap<std::string, types::mesh::ref> ext_gizmos::gizmos::mCachedMeshes{};

void ext_gizmos::gizmos::draw_infinity_grid(const glm::mat4 &view, const glm::mat4 &projection) {
	auto mesh{ get_infinity_grid_mesh() };
	if (mesh == nullptr) {
		spdlog::error("[{}] draw_infinity_grid: Failed to create mesh", class_name);
		return;
	}

	auto shader{ mesh->get_shader_program() };
	if (shader == nullptr) {
		spdlog::error("[{}] draw_infinity_grid: Failed to create shader", class_name);
		return;
	}

	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glDepthMask(GL_FALSE);
	shader->use();
	shader->set_uniform("u_view", view);
	shader->set_uniform("u_projection", projection);
	shader->unuse();

	mesh->draw();
	// glDepthMask(GL_TRUE);
	// glBlendFunc(GL_ONE, GL_ZERO);
}

types::mesh::ref ext_gizmos::gizmos::get_infinity_grid_mesh() {
	using namespace golxzn::types_literals;
	static const std::string name{ "infinity_grid_mesh" };
	if (auto mesh{ mCachedMeshes.find(name) }; mesh != std::end(mCachedMeshes)) {
		return mesh->second;
	}

	static constexpr glm::vec4 white{ 1.0_f16, 1.0_f16, 1.0_f16, 1.0_f16 };
	auto grid{ types::mesh::make("grid",
		{
			{ glm::vec3{ -1.0_f16, 0.0_f16,  1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{  1.0_f16, 0.0_f16,  1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{  1.0_f16, 0.0_f16, -1.0_f16 }, white, glm::vec2{} },

			{ glm::vec3{ -1.0_f16, 0.0_f16,  1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{  1.0_f16, 0.0_f16, -1.0_f16 }, white, glm::vec2{} },
			{ glm::vec3{ -1.0_f16, 0.0_f16, -1.0_f16 }, white, glm::vec2{} },
		}, {},
		get_infinity_grid_shader()
	) };
	if (grid != nullptr) {
		grid->get_mod<mods::mod_capabilities>()->set(mods::capabilities::blend);
		grid->get_mod<mods::blend::holder>()->set_function(
			mods::blend::function::src_alpha, mods::blend::function::one_minus_src_alpha);
		grid->get_mod<mods::depth::holder>()->set_mask(false);
		// grid->set_blend_func(blend_factors::src_alpha, blend_factors::one_minus_src_alpha);
		mCachedMeshes.emplace(name, grid);
	}

	return grid;
}

types::shader_program::ref ext_gizmos::gizmos::get_infinity_grid_shader() {
	static const std::string name{ "infinity_grid_shader" };
	if (auto shader{ mCachedShaders.find(name) }; shader != std::end(mCachedShaders)) {
		return shader->second;
	}

	auto shader = types::shader_program::make("grid_sp", {
		"res://shaders/grid/infinity_grid.vert",
		"res://shaders/grid/infinity_grid.frag",
	});
	if (shader != nullptr && program_status::link_success == shader->link()) {
		mCachedShaders.emplace(name, shader);
	}
	return shader;
}

} // namespace golxzn::graphics::extensions
