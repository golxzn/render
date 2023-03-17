#include <spdlog/spdlog.h>
#include "golxzn/graphics/types/mesh.hpp"

#include "golxzn/graphics/types/shader_program.hpp"
#include "golxzn/graphics/types/texture.hpp"
#include "golxzn/graphics/types/material.hpp"
#include "golxzn/graphics/controller/controller.hpp"

namespace golxzn::graphics::types {

mesh::mesh(const std::string &name,
		const std::vector<vertex> &vertices,
		const std::vector<core::u32> &indices,
		const core::sptr<shader_program> &shader_program,
		const core::sptr<material> &material,
		const core::umap<std::string, core::sptr<texture>> &textures)
	: named{ name }, mVertices{ vertices }, mIndices{ indices }, mShaderProgram{ shader_program }
	, mMaterial{ material }, mTextures{ textures } {
	generate();
}

mesh::mesh(const std::string &name,
		std::vector<vertex> &&vertices,
		std::vector<core::u32> &&indices,
		const core::sptr<shader_program> &shader_program,
		const core::sptr<material> &material,
		const core::umap<std::string, core::sptr<texture>> &textures)
	: named{ name }, mVertices{ std::move(vertices) }, mIndices{ std::move(indices) }
	, mShaderProgram{ shader_program }, mMaterial{ material }, mTextures{ textures } {
	generate();
}

void mesh::set_shader_program(const core::sptr<shader_program> &shader_program) {
	mShaderProgram = shader_program;
}

void mesh::set_vertices(std::vector<vertex> &&vertices, std::vector<core::u32> &&indices) {
	mVertices = std::move(vertices);
	mIndices = std::move(indices);
	generate();
}

void mesh::set_material(const core::sptr<material> &material) {
	mMaterial = material;
}

void mesh::add_texture(const std::string &name, core::sptr<texture> texture) {
	mTextures.emplace(name, texture);
}

void mesh::remove_texture(const std::string &name) {
	mTextures.erase(name);
}

void mesh::draw() {
	if (mShaderProgram == nullptr) return;

	mShaderProgram->use();

	update_shader_program_uniforms();
	if (auto api{ controller::api() }; api != nullptr) {
		api->draw_mesh(mObject);
	}

	mShaderProgram->unuse();
}

void mesh::generate() {
	auto api{ controller::api() };
	if (api == nullptr) {
		spdlog::error("[{}] Failed to generate mesh '{}', rendering API is not set!",
			class_name, full_name());
		return;
	}

	mObject = api->make_mesh(mVertices, mIndices);
	mObject->set_property("name", full_name());
}

void mesh::update_shader_program_uniforms() {

	if (mMaterial != nullptr) {
		mShaderProgram->set_uniform("material.ambient", mMaterial->ambient);
		mShaderProgram->set_uniform("material.diffuse", mMaterial->diffuse);
		mShaderProgram->set_uniform("material.specular", mMaterial->specular);
		mShaderProgram->set_uniform("material.shininess", mMaterial->shininess);
	}

	if (mTextures.size() > 0) {
		using namespace types_literals;
		auto texture_count{ 0_i32 };
		for (const auto &[name, texture] : mTextures) {
			if (texture != nullptr) {
				spdlog::info("[{}] Binding texture '{}' at index {} as '{}'",
					class_name, texture->full_name(), texture_count, name);
				mShaderProgram->set_uniform(name, texture_count);
				texture->bind(texture_count);
				texture_count++;
			}
		}
	}

}

} // namespace golxzn::graphics::types
