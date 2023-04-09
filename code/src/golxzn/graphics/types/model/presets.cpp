#include <glm/glm.hpp>
#include "golxzn/graphics/types/model/presets.hpp"

namespace golxzn::graphics::presets {

vertices_and_indices cube_map_vertices(const glm::vec3 color) noexcept {
	using namespace golxzn::types_literals;

	return vertices_and_indices{ std::vector<types::vertex>{
		{ glm::vec3{ -1.0_f16,  1.0_f16, -1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{ -1.0_f16, -1.0_f16, -1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{  1.0_f16, -1.0_f16, -1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{  1.0_f16, -1.0_f16, -1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{  1.0_f16,  1.0_f16, -1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{ -1.0_f16,  1.0_f16, -1.0_f16 }, color, glm::vec2{} },

		{ glm::vec3{ -1.0_f16, -1.0_f16,  1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{ -1.0_f16, -1.0_f16, -1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{ -1.0_f16,  1.0_f16, -1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{ -1.0_f16,  1.0_f16, -1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{ -1.0_f16,  1.0_f16,  1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{ -1.0_f16, -1.0_f16,  1.0_f16 }, color, glm::vec2{} },

		{ glm::vec3{  1.0_f16, -1.0_f16, -1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{  1.0_f16, -1.0_f16,  1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{  1.0_f16,  1.0_f16,  1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{  1.0_f16,  1.0_f16,  1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{  1.0_f16,  1.0_f16, -1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{  1.0_f16, -1.0_f16, -1.0_f16 }, color, glm::vec2{} },

		{ glm::vec3{ -1.0_f16, -1.0_f16,  1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{ -1.0_f16,  1.0_f16,  1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{  1.0_f16,  1.0_f16,  1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{  1.0_f16,  1.0_f16,  1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{  1.0_f16, -1.0_f16,  1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{ -1.0_f16, -1.0_f16,  1.0_f16 }, color, glm::vec2{} },

		{ glm::vec3{ -1.0_f16,  1.0_f16, -1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{  1.0_f16,  1.0_f16, -1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{  1.0_f16,  1.0_f16,  1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{  1.0_f16,  1.0_f16,  1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{ -1.0_f16,  1.0_f16,  1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{ -1.0_f16,  1.0_f16, -1.0_f16 }, color, glm::vec2{} },

		{ glm::vec3{ -1.0_f16, -1.0_f16, -1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{ -1.0_f16, -1.0_f16,  1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{  1.0_f16, -1.0_f16, -1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{  1.0_f16, -1.0_f16, -1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{ -1.0_f16, -1.0_f16,  1.0_f16 }, color, glm::vec2{} },
		{ glm::vec3{  1.0_f16, -1.0_f16,  1.0_f16 }, color, glm::vec2{} },
	}, {}
	};
}

vertices_and_indices cube_vertices(const glm::vec3 color, const core::f16 scale) noexcept {
	using namespace golxzn::types_literals;

	const auto sc{ std::abs(scale) };
	return vertices_and_indices{
		std::vector<types::vertex>{
			{ glm::vec3{ -sc, -sc,  sc }, color, glm::vec2{} },
			{ glm::vec3{ -sc,  sc,  sc }, color, glm::vec2{} },
			{ glm::vec3{  sc,  sc,  sc }, color, glm::vec2{} },
			{ glm::vec3{  sc, -sc,  sc }, color, glm::vec2{} },

			{ glm::vec3{ -sc, -sc, -sc }, color, glm::vec2{} },
			{ glm::vec3{  sc, -sc, -sc }, color, glm::vec2{} },
			{ glm::vec3{  sc,  sc, -sc }, color, glm::vec2{} },
			{ glm::vec3{ -sc,  sc, -sc }, color, glm::vec2{} },

			{ glm::vec3{ -sc, -sc, -sc }, color, glm::vec2{} },
			{ glm::vec3{ -sc, -sc,  sc }, color, glm::vec2{} },
			{ glm::vec3{ -sc,  sc,  sc }, color, glm::vec2{} },
			{ glm::vec3{ -sc,  sc, -sc }, color, glm::vec2{} },

			{ glm::vec3{  sc, -sc, -sc }, color, glm::vec2{} },
			{ glm::vec3{  sc,  sc, -sc }, color, glm::vec2{} },
			{ glm::vec3{  sc,  sc,  sc }, color, glm::vec2{} },
			{ glm::vec3{  sc, -sc,  sc }, color, glm::vec2{} },

			{ glm::vec3{ -sc, -sc, -sc }, color, glm::vec2{} },
			{ glm::vec3{  sc, -sc, -sc }, color, glm::vec2{} },
			{ glm::vec3{  sc, -sc,  sc }, color, glm::vec2{} },
			{ glm::vec3{ -sc, -sc,  sc }, color, glm::vec2{} },

			{ glm::vec3{ -sc,  sc, -sc }, color, glm::vec2{} },
			{ glm::vec3{ -sc,  sc,  sc }, color, glm::vec2{} },
			{ glm::vec3{  sc,  sc,  sc }, color, glm::vec2{} },
			{ glm::vec3{  sc,  sc, -sc }, color, glm::vec2{} },
		},
		{
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 9, 10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20
		}
	};
}
vertices_and_indices plane_vertices(const glm::vec3 color, const glm::vec3 o) noexcept {
	using namespace golxzn::types_literals;

	return vertices_and_indices{
		std::vector<types::vertex>{
			{ glm::vec3{ -o.x, -o.y, o.z }, color, glm::vec2{ 0.0, 0.0 } },
			{ glm::vec3{ -o.x,  o.y, o.z }, color, glm::vec2{ 0.0, 1.0 } },
			{ glm::vec3{  o.x,  o.y, o.z }, color, glm::vec2{ 1.0, 1.0 } },
			{ glm::vec3{  o.x, -o.y, o.z }, color, glm::vec2{ 1.0, 0.0 } },
		},
		{
			0, 1, 2,
			0, 2, 3,
		},
	};
}

} // namespace golxzn::graphics::presets
