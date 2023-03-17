#include <algorithm>
#include <spdlog/spdlog.h>
#include <golxzn/core/utils/traits.hpp>
#include "golxzn/graphics/types/material.hpp"

namespace golxzn::graphics::types {

const material material::default_material{ "default",
	glm::vec3{ 0.2f, 0.2f, 0.2f },
	glm::vec3{ 0.5f, 0.5f, 0.5f },
	glm::vec3{ 1.0f, 1.0f, 1.0f },
	32.0f
};

material::material(const std::string &name, const glm::vec3 &ambient, const glm::vec3 &diffuse,
	const glm::vec3 &specular, const core::f16 shininess)
	: named{ name }, ambient{ ambient }, diffuse{ diffuse }
	, specular{ specular }, shininess{ shininess } {}

material material::from_string(const std::string &str) {
	if (str.size() < smallest_string_length) {
		spdlog::warn("[material::from_string] string size {} is less than smallest string length {}",
			str.size(), smallest_string_length);
		return default_material;
	}

	static constexpr auto block_delimeter{ ':' };
	static constexpr auto vec_delimiter{ ',' };

	if (std::count(std::begin(str), std::end(str), vec_delimiter) != 6) {
		spdlog::warn("[material::from_string] Invalid string format: "
			"string '{}' does not have 6 vector delimiters ({})", str, vec_delimiter);
		return default_material;
	}
	if (std::count(std::begin(str), std::end(str), block_delimeter) != 2) {
		spdlog::warn("[material::from_string] Invalid string format: "
			"string '{}' does not have 2 blocks delimiters ({})", str, block_delimeter);
		return default_material;
	}

	static const auto parse_float{ [] (const std::string_view str) -> core::f16 {
		static const auto is_number{ [] (const char c) -> bool {
			return std::isdigit(c) || core::traits::any_from(c, '-', '+', '.');
		} };
		if (std::all_of(std::begin(str), std::end(str), is_number)) {
			return std::strtof(str.data(), nullptr);
		}
		return core::f16{};
	} };

	static const auto parse_vector{ [] (const std::string_view str) {
		const auto first_delim_pos{ str.find(vec_delimiter) };
		const auto last_delim_pos{ str.rfind(vec_delimiter) };
		if (first_delim_pos == last_delim_pos) {
			return glm::vec3{};
		}

		return glm::vec3{
			parse_float(str.substr(0, first_delim_pos)),
			parse_float(str.substr(first_delim_pos + 1, last_delim_pos - first_delim_pos)),
			parse_float(str.substr(last_delim_pos + 1, str.size() - last_delim_pos))
		};
	} };

	const auto name_delim_pos{ str.find(block_delimeter) };
	const auto block_0_delim_pos{ str.find(block_delimeter, name_delim_pos + 1) };
	const auto block_1_delim_pos{ str.find(block_delimeter, block_0_delim_pos + 1) };
	const auto block_2_delim_pos{ str.find(block_delimeter, block_1_delim_pos + 1) };

	const auto name_block{ str.substr(0, name_delim_pos) };
	const auto block_0{ str.substr(name_delim_pos + 1, block_0_delim_pos) };
	const auto block_1{ str.substr(block_0_delim_pos + 1, block_1_delim_pos) };
	const auto block_2{ str.substr(block_1_delim_pos + 1, block_2_delim_pos) };
	const auto shininess_block{ str.substr(block_2_delim_pos + 1) };

	return material{
		std::string{ name_block },
		parse_vector(block_0),
		parse_vector(block_1),
		parse_vector(block_2),
		parse_float(shininess_block)
	};
}

std::string material::to_string(const material &mat) {
	std::string str;
	str += mat.name();
	str += ':';
	str += std::to_string(mat.ambient.x);
	str += ',';
	str += std::to_string(mat.ambient.y);
	str += ',';
	str += std::to_string(mat.ambient.z);
	str += ':';
	str += std::to_string(mat.diffuse.x);
	str += ',';
	str += std::to_string(mat.diffuse.y);
	str += ',';
	str += std::to_string(mat.diffuse.z);
	str += ':';
	str += std::to_string(mat.specular.x);
	str += ',';
	str += std::to_string(mat.specular.y);
	str += ',';
	str += std::to_string(mat.specular.z);
	str += ':';
	str += std::to_string(mat.shininess);
	return str;
}

material material::from_binary(const core::bytes &bytes) {
	return from_binary(bytes.data(), bytes.size());
}

core::bytes material::to_binary(const material &mat) {
	core::bytes bytes(sizeof(material), '\0');
	std::memcpy(bytes.data(), &mat, sizeof(material));
	return bytes;
}

std::vector<material> material::from_binaries(const core::bytes &bytes) {
	if (bytes.size() % sizeof(material) != 0) {
		spdlog::warn("[material::from_binaries] Bytes size {} is not a multiple of material size {}",
			bytes.size(), sizeof(material));
		return {};
	}
	std::vector<material> materials;
	materials.reserve(bytes.size() / sizeof(material));
	for (core::size i{ 0 }; i < bytes.size(); i += sizeof(material)) {
		materials.emplace_back(from_binary(&bytes[i], sizeof(material)));
	}
	return materials;
}

material material::from_binary(const core::u8 *bytes, const core::usize len) {
	if (len != sizeof(material)) {
		spdlog::warn("[material::from_binary] Bytes size {} is not equal to material size {}",
			len, sizeof(material));
		return default_material;
	}

	material mat{ default_material };
	std::memcpy(&mat, bytes, sizeof(material));
	return mat;
}

} // namespace golxzn::graphics::types
