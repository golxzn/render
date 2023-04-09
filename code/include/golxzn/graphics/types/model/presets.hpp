#pragma once

#include <glm/vec4.hpp>
#include "golxzn/graphics/types/model/mesh.hpp"

namespace golxzn::graphics::presets {

static constexpr glm::vec3 default_color{ 1.0f, 1.0f, 1.0f };

struct vertices_and_indices {
	std::vector<types::vertex> vertices;
	std::vector<core::u32> indices;
};

vertices_and_indices cube_map_vertices(const glm::vec3 color = default_color) noexcept;
vertices_and_indices cube_vertices(const glm::vec3 color = default_color, const core::f16 scale = 1.0f) noexcept;
vertices_and_indices plane_vertices(
	const glm::vec3 color = default_color,
	const glm::vec3 offset = { 1.0f, 1.0f, 0.0f }) noexcept;

} // namespace golxzn::graphics::presets
