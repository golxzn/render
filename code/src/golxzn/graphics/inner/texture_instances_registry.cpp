#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <golxzn/core/utils/strutils.hpp>
#include <golxzn/core/resources/manager.hpp>

#include "golxzn/graphics/inner/texture_instances_registry.hpp"

namespace golxzn::graphics::inner {

texture_instances_registry::storage_policy texture_instances_registry::policy{ texture_instances_registry::storage_policy::with_cache };
core::usize texture_instances_registry::cache_size{ 0 };
core::usize texture_instances_registry::max_cache_size{ texture_instances_registry::default_max_cache_size };
core::umap<std::string, types::texture::ref> texture_instances_registry::cache{};

types::texture::ref texture_instances_registry::get_or_create(const types::texture::type tex_type, const std::string &path) {
	if (auto tex{ load_from_cache(path) }; tex != nullptr) return tex;
	using namespace types;

	types::texture::ref tex;
	core::usize data_length{};
	switch (tex_type) {
		case texture::type::texture_2d: tex = load_texture_2D(path, data_length); break;
		case texture::type::cube_map: tex = load_cube_map(path, data_length); break;
		case texture::type::texture_gif: tex = load_texture_gif(path, data_length); break;
		default:
			/// @todo: Use magic_enum for target
			spdlog::error("{}: Texture type not supported: {}", class_name, static_cast<core::u32>(tex_type));
			return nullptr;
	}
	if (tex) {
		tex->set_path(path);
		cache_texture(tex, path, data_length);
	}
	return tex;
}

texture_instances_registry::storage_policy texture_instances_registry::get_storage_policy() noexcept {
	return policy;
}
core::usize texture_instances_registry::get_max_cache_size() noexcept {
	return max_cache_size;
}
core::usize texture_instances_registry::get_cache_size() noexcept {
	return cache_size;
}

void texture_instances_registry::set_storage_policy(const storage_policy _policy) {
	policy = _policy;
}
void texture_instances_registry::set_max_cache_size(const core::usize size) {
	max_cache_size = size;
}

core::usize texture_instances_registry::delete_unused() {
	if (cache.empty()) {
		return 0;
	}

	core::usize deleted{};
	for (const auto &[path, tex] : cache) {
		if (tex == nullptr) {
			cache.erase(path);
			continue;
		}
		if (tex.use_count() > 1) {
			continue;
		}
		const auto size{ tex->bytes_count() };
		cache.erase(path);
		cache_size -= size;
		++deleted;
	}
	return deleted;
}

void texture_instances_registry::clean() {
	cache.clear();
	cache_size = 0;
}

types::texture::ref texture_instances_registry::load_texture_2D(const std::string &path, core::usize &length) {
	if (path.empty()) return nullptr;

	if (auto img{ core::res_man::load_image(path) }; img != nullptr) {
		length = img->bytes_count();

		auto tex{ std::make_shared<types::texture>(core::fs::path{ path }.stem().string()) };
		tex->param<types::texture::min_filter>() = types::texture::min_filter::filter::nearest;
		tex->param<types::texture::mag_filter>() = types::texture::mag_filter::filter::linear;
		tex->set_image(types::tex_target::texture_2d, img);

		return tex;
	}
	return nullptr;
}

types::texture::ref texture_instances_registry::load_texture_gif(const std::string &path, core::usize &length) {
	if (path.empty()) return nullptr;

	/// @todo: Implement GIF loading
	spdlog::error("{}: GIF textures are not supported yet :,<", class_name);
	return nullptr;
}

types::texture::ref texture_instances_registry::load_cube_map(const std::string &path, core::usize &length) {
	if (path.empty()) return nullptr;
	using namespace types;
	using namespace std::string_view_literals;

	static constexpr std::initializer_list faces{
		"right"sv,   "left"sv,
		"top"sv,     "bottom"sv,
		"front"sv,   "back"sv
	};
	static constexpr std::initializer_list targets{
		tex_target::texture_cube_map_positive_x, tex_target::texture_cube_map_negative_x,
		tex_target::texture_cube_map_positive_y, tex_target::texture_cube_map_negative_y,
		tex_target::texture_cube_map_positive_z, tex_target::texture_cube_map_negative_z
	};

	static const auto ext_and_dir = [](std::string_view name) {
		if (auto dot_pos{ name.find_last_of('.') }; dot_pos != std::string_view::npos) {
			return std::make_pair(name.substr(0, dot_pos), name.substr(dot_pos));
		}
		return std::make_pair(""sv, ""sv);
	};

	const auto [dir, ext]{ ext_and_dir(path) };

	texture::cubemap_array<core::types::image::ref> images;

	bool loaded{ true };
	std::transform(std::cbegin(faces), std::cend(faces), std::begin(images),
		[&](const auto &face) -> core::types::image::ref {
			if (!loaded) return nullptr;

			const auto path{ core::utils::concat(dir, "/", face, ext) };
			auto img{ core::res_man::load_image(path) };
			loaded = img != nullptr;
			if (!loaded) {
				spdlog::error("[{}] The {} face of the cube map hasn't been loaded by path '{}'.",
					class_name, face, path);
			}
			return img;
		}
	);

	if (!loaded) return nullptr;

	auto tex{ std::make_shared<texture>(core::fs::path{ path }.stem().string(), texture::type::cube_map) };
	length = 0;
	auto target_iter{ std::cbegin(targets) };
	for (auto &img : images) {
		length += img->bytes_count();
		tex->set_image(*target_iter, img, tex_format::RGB_8);
		++target_iter;
	}

	using namespace types;

	tex->param<texture::mag_filter>() = texture::mag_filter::filter::linear;
	tex->param<texture::min_filter>() = texture::min_filter::filter::linear;
	auto wrap{ tex->param<texture::wrap>() };
	wrap[texture::wrap::type::s] = texture::wrap::mode::clamp_to_edge;
	wrap[texture::wrap::type::t] = texture::wrap::mode::clamp_to_edge;
	wrap[texture::wrap::type::r] = texture::wrap::mode::clamp_to_edge;

	return tex;
}



types::texture::ref texture_instances_registry::load_from_cache(const std::string &path) {
	if (policy != storage_policy::with_cache) return nullptr;

	if (auto found{ cache.find(path) }; found != std::end(cache)) {
		/// @todo: return found->second->clone ?
		return found->second;
	}
	return nullptr;
}

void texture_instances_registry::cache_texture(types::texture::ref tex, const std::string &name, const core::usize data_length) {
	if (policy != storage_policy::with_cache) return;

	if (cache_size < max_cache_size && cache_size + data_length > max_cache_size) {
		const auto deleted{ delete_unused() };
		spdlog::warn("[{}] The cache size was exceeded. Deleted {} textures.", class_name, deleted);
	}
	if (cache_size < max_cache_size && cache_size + data_length > max_cache_size) {
		spdlog::error("[{}] The cache size is exceeded. The image '{}' will not be cached.", class_name, name);
		return;
	}

	tex->set_bytes_count(data_length);
	cache.insert_or_assign(name, tex);
	cache_size += data_length;
}


} // namespace golxzn::graphics::inner
