#pragma once

#include <golxzn/core/types.hpp>

namespace golxzn::render {
class Mesh;
} // namespace golxzn::render

namespace golxzn::render::extensions {

template<class RenderEngine>
struct primitives {
	using type = primitives<RenderEngine>;
	using engine = RenderEngine;

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

template<class RenderEngine>
void primitives<RenderEngine>::draw_triangle(
	const core::f32 x1, const core::f32 y1,
	const core::f32 x2, const core::f32 y2,
	const core::f32 x3, const core::f32 y3) {

	using namespace core::types_literals;

	draw_triangle(
		x1, 0._f32, y1,
		x2, 0._f32, y2,
		x3, 0._f32, y3
	);
}

template<class RenderEngine>
void primitives<RenderEngine>::draw_rect(
	const core::f32 x, const core::f32 y,
	const core::f32 width, const core::f32 height) {

	using namespace core::types_literals;

	draw_plane(
		x, 0._f32, y,
		width, height
	);
}

template<class RenderEngine>
void primitives<RenderEngine>::draw_circle(
	const core::f32 x, const core::f32 y,
	const core::f32 radius, const core::u32 steps) {

	using namespace core::types_literals;

	/// @todo: Implement me
}

template<class RenderEngine>
void primitives<RenderEngine>::draw_line(
	const core::f32 x1, const core::f32 y1,
	const core::f32 x2, const core::f32 y2) {

	/// @todo: Implement me
}


template<class RenderEngine>
static void primitives<RenderEngine>::draw_triangle(
	const core::f32 x1, const core::f32 y1, const core::f32 z1,
	const core::f32 x2, const core::f32 y2, const core::f32 z2,
	const core::f32 x3, const core::f32 y3, const core::f32 z3) {

	using namespace core::types_literals;

	auto mesh{ engine::make_mesh({
		x1, y1, z1,
		x2, y2, z2,
		x3, y3, z3
	}, { 0_u32, 1_u32, 2_u32 }) };

	engine::draw_mesh(mesh, engine::mode::triangle);
}

template<class RenderEngine>
static void primitives<RenderEngine>::draw_plane(
	const core::f32 x, const core::f32 y, const core::f32 z,
	const core::f32 width, const core::f32 height) {

	using namespace core::types_literals;

	auto mesh{ engine::make_mesh({
		x,         y, z,
		x,         y, z + height,
		x + width, y, z + height,
		x + width, y, z
	}, {
		0_u32, 1_u32, 2_u32,
		0_u32, 2_u32, 3_u32
	}) };
	engine::draw_mesh(mesh, engine::mode::triangle);
}

template<class RenderEngine>
static void primitives<RenderEngine>::draw_cube(
	const core::f32 x, const core::f32 y, const core::f32 z,
	const core::f32 width, const core::f32 height, const core::f32 depth) {

	/// @todo: Implement me
}

template<class RenderEngine>
static void primitives<RenderEngine>::draw_sphere(
	const core::f32 x, const core::f32 y, const core::f32 z,
	const core::f32 radius, const core::u32 steps) {

	/// @todo: Implement me
}



} // namespace golxzn::render::extensions
