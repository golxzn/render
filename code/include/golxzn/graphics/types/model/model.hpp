#pragma once

#include <glm/mat4x4.hpp>
#include "golxzn/graphics/types/object.hpp"
#include "golxzn/graphics/types/name_manager.hpp"
#include "golxzn/graphics/mods/capabilities.hpp"

#include "golxzn/graphics/types/model/mesh.hpp"

namespace golxzn::graphics::types {

// class model : named, public mods::holder {
// 	static constexpr std::string_view class_name{ "graphics::types::model" };
// public:
// 	static constexpr glm::mat4 default_transform{ 1.0f };
// 	using ref = core::sptr<model>;

// 	explicit model(const std::string &name, const glm::mat4 &transform = default_transform);
// 	model(const std::string &name, const std::vector<mesh::ref> &meshes, const glm::mat4 &transform = default_transform);
// 	model(const std::string &name, std::vector<mesh::ref> &&meshes, const glm::mat4 &transform = default_transform);

// 	void draw();

// 	bool add_mesh(const mesh::ref &mesh);
// 	mesh::ref remove_mesh(const mesh::ref &mesh);

// 	bool valid() const noexcept;
// 	const glm::mat4 &transform() const noexcept;
// 	glm::mat4 &transform() noexcept;
// 	const std::vector<mesh::ref> &meshes() const noexcept;
// 	std::vector<mesh::ref> &meshes() noexcept;

// 	void set_transform(const glm::mat4 &transform);

// private:
// 	object::ref m_object;
// 	std::vector<mesh::ref> m_meshes;
// 	glm::mat4 m_transform;
// };

} // namespace golxzn::graphics::types
