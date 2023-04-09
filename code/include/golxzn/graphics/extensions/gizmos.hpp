#pragma once

#include <glm/glm.hpp>

#include "golxzn/graphics/types/shader/program.hpp"
#include "golxzn/graphics/types/model/mesh.hpp"

namespace golxzn::graphics::extensions {

struct ext_gizmos {
	static constexpr std::string_view class_name{ "extensions::gizmos" };
	using type = ext_gizmos;

	GOLXZN_STATIC_CLASS(ext_gizmos);

	class gizmos {
	public:
		GOLXZN_STATIC_CLASS(gizmos);

		static void draw_infinity_grid(const glm::mat4 &view, const glm::mat4 &projection);

	private:
		static core::umap<std::string, types::shader_program::ref> mCachedShaders;
		static core::umap<std::string, types::mesh::ref> mCachedMeshes;

		static types::mesh::ref get_infinity_grid_mesh();
		static types::shader_program::ref get_infinity_grid_shader();
	};
};

} // namespace golxzn::graphics::extensions
