#pragma once

#include <glm/fwd.hpp>
#include "golxzn/graphics/types/texture/texture.hpp"

namespace golxzn::graphics::inner {

class texture_instances_registry {
	static constexpr std::string_view class_name{ "graphics::inner::texture_instances_registry" };
public:
	static constexpr core::usize default_max_cache_size{ 2'147'483'648 };
	GOLXZN_STATIC_CLASS(texture_instances_registry);

	enum class storage_policy { with_cache, without_cache };

	static types::texture::ref get_or_create(const types::texture::type tex_type, const std::string &path);



	static storage_policy get_storage_policy() noexcept;
	static core::usize get_max_cache_size() noexcept;
	static core::usize get_cache_size() noexcept;

	static void set_storage_policy(const storage_policy policy);
	static void set_max_cache_size(const core::usize size);

	static core::usize delete_unused();
	static void clean();

private:
	static storage_policy policy;
	static core::usize cache_size;
	static core::usize max_cache_size;
	static core::umap<std::string, types::texture::ref> cache;

	static types::texture::ref load_texture_2D(const std::string &path, core::usize &length);
	static types::texture::ref load_texture_gif(const std::string &path, core::usize &length);
	static types::texture::ref load_cube_map(const std::string &path, core::usize &length);

	static types::texture::ref load_from_cache(const std::string &path);
	static void cache_texture(types::texture::ref texture, const std::string &name, const core::usize length);

};

} // namespace golxzn::graphics::inner
