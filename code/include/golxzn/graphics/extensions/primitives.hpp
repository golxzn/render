#pragma once

#include <golxzn/core/types.hpp>

namespace golxzn::graphics {
class Mesh;
} // namespace golxzn::graphics

/** @addtogroup graphics::extensions
 * @{
 */

namespace golxzn::graphics::extensions {

struct primitives {
	using type = primitives;

	static constexpr core::u32 default_circle_steps{ 10 };

	/**
	 * @defgroup 2D Primitives
	 * @{
	 */

	static void draw_triangle(
		const core::f32 x1, const core::f32 y1,
		const core::f32 x2, const core::f32 y2,
		const core::f32 x3, const core::f32 y3);

	static void draw_rect(
		const core::f32 x, const core::f32 y,
		const core::f32 width, const core::f32 height);

	static void draw_circle(
		const core::f32 x, const core::f32 y,
		const core::f32 radius, const core::u32 steps = default_circle_steps);

	static void draw_line(
		const core::f32 x1, const core::f32 y1,
		const core::f32 x2, const core::f32 y2);

	/** @} 2D Primitives */

	/**
	 * @defgroup 3D Primitives
	 * @{
	 */

	static void draw_triangle(
		const core::f32 x1, const core::f32 y1, const core::f32 z1,
		const core::f32 x2, const core::f32 y2, const core::f32 z2,
		const core::f32 x3, const core::f32 y3, const core::f32 z3);

	static void draw_plane(
		const core::f32 x, const core::f32 y, const core::f32 z,
		const core::f32 width, const core::f32 height);

	static void draw_cube(
		const core::f32 x, const core::f32 y, const core::f32 z,
		const core::f32 width, const core::f32 height, const core::f32 depth);

	static void draw_sphere(
		const core::f32 x, const core::f32 y, const core::f32 z,
		const core::f32 radius, const core::u32 steps = default_circle_steps);

	/** @} 3D Primitives */
};

} // namespace golxzn::graphics::extensions

/** @} graphics::extensions */

namespace golxzn::graphics {
namespace ext = extensions;
} // namespace golxzn::graphics
