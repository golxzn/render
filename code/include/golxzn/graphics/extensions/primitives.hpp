#pragma once

#include <glm/glm.hpp>

#include "golxzn/graphics/types/shader/program.hpp"
#include "golxzn/graphics/types/model/mesh.hpp"

/** @addtogroup graphics::extensions
 * @{
 */

namespace golxzn::graphics::extensions {

class primitives {
	static constexpr std::string_view class_name{ "extensions::primitives" };
public:
	using type = primitives;

	static constexpr glm::vec4 default_color{ 1.0f };
	static constexpr core::f16 default_line_width{ 0.01f };
	static constexpr core::u32 default_circle_steps{ 16 };

	/**
	 * @defgroup 2D Primitives
	 * @{
	 */

	static void draw_line(const glm::vec2 &start, const glm::vec2 &end,
		const core::f16 width = default_line_width,
		const glm::vec4 &color = default_color);

	// static void draw_triangle(const glm::vec2 &v1, const glm::vec2 &v2, const glm::vec2 &v3);

	static void draw_rectangle(const glm::vec2 &position, const glm::vec2 &size,
		const glm::vec4 &color = default_color);

	static void draw_circle(const glm::vec2 &center, const core::f16 radius,
		const glm::vec4 &color = default_color,
		const core::u32 steps = default_circle_steps);

	/** @} 2D Primitives */

	/**
	 * @defgroup 3D Primitives
	 * @{
	 */

	// static void draw_line(const glm::vec3 &start, const glm::vec3 &end,
		// const glm::vec4 &color = default_color,
		// const core::f16 width = default_line_width);

	// static void draw_triangle(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3);

	// static void draw_plane(const glm::vec3 &position, const glm::vec2 &size);

	// static void draw_cube(const glm::vec3 &position, const glm::vec3 &size);

	// static void draw_sphere(const glm::vec3 &center, const core::f16 radius,
	// 	const core::u32 steps = default_circle_steps);

	/** @} 3D Primitives */

private:
	static core::umap<std::string, types::shader_program::ref> mCachedShaders;
	static core::umap<std::string, types::mesh::ref> mCachedMeshes;

	static types::mesh::ref get_line_mesh(const core::u32 steps);
	static types::mesh::ref get_rect_mesh();
	static types::mesh::ref get_circle_mesh(const core::u32 steps);

	static types::shader_program::ref load_shader(const std::string &name);
	static types::shader_program::ref get_default_2d_shader();
};

} // namespace golxzn::graphics::extensions

/** @} graphics::extensions */

namespace golxzn::graphics {
namespace ext = extensions;
} // namespace golxzn::graphics
