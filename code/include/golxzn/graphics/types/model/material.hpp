#pragma once

#include <glm/glm.hpp>
#include <golxzn/core/aliases.hpp>
#include "golxzn/graphics/types/name_manager.hpp"

namespace golxzn::graphics::types {

struct material : public named {
	static constexpr auto smallest_string_length{ sizeof("a:0,0,0:0,0,0:0,0,0:0") - 1 };
	static const material default_material;

	using ref = std::shared_ptr<material>;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	core::f16 shininess;

	static ref make(const std::string &name, const glm::vec3 &ambient, const glm::vec3 &diffuse,
		const glm::vec3 &specular, const core::f16 shininess);
	static ref make(const std::string &str);

	material(const std::string &name, const glm::vec3 &ambient, const glm::vec3 &diffuse,
		const glm::vec3 &specular, const core::f16 shininess);

	/**
	 * @brief Parse material from string
	 * @param str string with the following format:
	 * `name:ambient.r,ambient.g,ambient.b:diffuse.r,diffuse.g,diffuse.b:specular.r,specular.g,specular.b:shininess<eol>`
	 * @return material
	 */
	static material from_string(const std::string &str);
	static std::string to_string(const material &mat);

	static material from_binary(const core::bytes &bytes);
	static core::bytes to_binary(const material &mat);

	static std::vector<material> from_binaries(const core::bytes &bytes);

private:
	static material from_binary(const core::u8 *bytes, const core::usize len);
};

} // namespace golxzn::graphics::types

