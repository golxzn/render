#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

#include "golxzn/graphics/inner/texture_instances_registry.hpp"

namespace golxzn::graphics::inner {

texture_instances_registry::storage_policy texture_instances_registry::policy{ texture_instances_registry::storage_policy::with_cache };
core::usize texture_instances_registry::cache_size{ 0 };
core::usize texture_instances_registry::max_cache_size{ texture_instances_registry::default_max_cache_size };
core::umap<std::string, types::texture::ref> texture_instances_registry::cache{};

types::texture::ref texture_instances_registry::get_or_create(const types::texture::type tex_type, const std::string &path) {
	return nullptr;
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
		const auto size{ tex->length() };
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

void texture_instances_registry::cache_texture(const types::texture::ref &tex, const std::string &name,
		const glm::i32vec2 &size, const core::i32 channels) {
	if (policy != storage_policy::with_cache) {
		return;
	}

	const core::usize data_length{ static_cast<core::usize>(size.x * size.y * channels) };
	if (cache_size < max_cache_size && cache_size + data_length > max_cache_size) {
		const auto deleted{ delete_unused() };
		spdlog::warn("[{}] The cache size was exceeded. Deleted {} textures.", class_name, deleted);
	}
	if (cache_size < max_cache_size && cache_size + data_length > max_cache_size) {
		spdlog::error("[{}] The cache size is exceeded. The image '{}' will not be cached.", class_name, name);
		return;
	}

	cache.insert_or_assign(name, tex);
	cache_size += data_length;
}


} // namespace golxzn::graphics::inner
