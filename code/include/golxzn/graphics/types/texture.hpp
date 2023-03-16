#pragma once

#include <array>
#include <golxzn/core/types/color.hpp>

#include "golxzn/graphics/types/object.hpp"
#include "golxzn/graphics/types/name_manager.hpp"

namespace golxzn::graphics::types {

class texture : public named {
	static constexpr const std::string_view class_name{ "graphics::types::texture" };
public:
	static constexpr core::u32 cube_map_faces{ 6 };
	static constexpr std::string_view param_path{ "path" };
	static constexpr std::string_view param_data{ "data" };
	static constexpr std::string_view param_type{ "type" };
	static constexpr std::string_view param_width{ "width" };
	static constexpr std::string_view param_height{ "height" };

	using ref = std::shared_ptr<texture>;
	using bytes = std::vector<core::u8>;
	template<class T>
	using cubemap_array = std::array<T, cube_map_faces>;

	struct parameter;
	struct depth_stencil_texture_mode;
	struct base_level;
	struct compare_function;
	struct compare_mode;
	struct min_filter;
	struct mag_filter;
	struct max_level;
	struct lod;
	struct swizzle;
	struct wrap;

	template<class T, std::enable_if_t<std::is_base_of_v<parameter, T>, bool> = false>
	T param() const noexcept { return T{ mObject }; }

	enum class type {
		invalid,
		texture_2d,
		cube_map
	};

	static ref make(const type tex_type, const std::string &path);
	static ref make(const std::string &name, bytes &&data);
	static ref make(const std::string &name, cubemap_array<bytes> &&data);

	explicit texture(const type tex_type, const std::string &path);
	explicit texture(const std::string &name, bytes &&data);
	explicit texture(const std::string &name, cubemap_array<bytes> &&data);

	void bind(const core::u32 unit = 0) const noexcept;
	void unbind() const noexcept;

	type get_type() const noexcept;
	bool valid() const noexcept;
	object::id_t id() const noexcept;
	core::u32 width() const noexcept;
	core::u32 height() const noexcept;

	void set_data(const bytes &data);
	void set_data(bytes &&data);
	void set_data(const cubemap_array<bytes> &data);
	void set_data(cubemap_array<bytes> &&data);

	bool generate(const bool setup_default_params = true);
	bool generate_mip_maps();

	void set_border_color(const core::color &color) noexcept;

private:
	object::ref mObject;

	bool make_texture(const type tex_type);
};

//====================================== texture parameters ======================================//

struct texture::parameter {
	static constexpr std::string_view class_name{ "graphics::types::texture::parameter" };

	core::wptr<object> texture_object;
	explicit parameter(object::ref object_ref) noexcept;

	object::ref get_object() const noexcept;
};

struct texture::depth_stencil_texture_mode : public texture::parameter {
	using parameter::parameter;

	enum class mode {
		stencil_index,
		depth_component,
		depth_stencil
	};
	depth_stencil_texture_mode &operator=(const mode value);
};

struct texture::base_level : public texture::parameter {
	using parameter::parameter;
	base_level &operator=(const core::u32 level);
};

struct texture::compare_function : public texture::parameter {
	using parameter::parameter;

	enum class function {
		never,
		less,
		equal,
		lequal,
		greater,
		notequal,
		gequal,
		always
	};
	compare_function &operator=(const function value);
};

struct texture::compare_mode : public texture::parameter {
	using parameter::parameter;

	enum class mode {
		none,
		compare_ref_to_texture
	};
	compare_mode &operator=(const mode value);
};

struct texture::min_filter : public texture::parameter {
	using parameter::parameter;

	enum class filter {
		nearest,
		linear,
		nearest_mipmap_nearest,
		linear_mipmap_nearest,
		nearest_mipmap_linear,
		linear_mipmap_linear
	};
	min_filter &operator=(const filter value);
};

struct texture::mag_filter : public texture::parameter {
	using parameter::parameter;

	enum class filter {
		nearest,
		linear
	};
	mag_filter &operator=(const filter value);
};

struct texture::max_level : public texture::parameter {
	using parameter::parameter;
	max_level &operator=(const core::u32 value);
};

struct texture::lod : public texture::parameter {
	using parameter::parameter;
	struct min_level : texture::parameter {};
	struct max_level : texture::parameter {};
	struct bias : texture::parameter {};

	enum class level { min, max, bias };
	level current_mode{ level::bias };

	lod &operator[](const level new_mode) noexcept;
	lod &operator=(const core::f32 value);
};

struct texture::swizzle : public texture::parameter {
	using parameter::parameter;
	struct red_type : texture::parameter {};
	struct green_type : texture::parameter {};
	struct blue_type : texture::parameter {};
	struct alpha_type : texture::parameter {};

	enum class type { red, green, blue, alpha };
	type current_type{ type::red };

	enum class channel {
		red,
		green,
		blue,
		alpha,
		zero,
		one
	};
	swizzle &operator[](const type new_type) noexcept;
	swizzle &operator=(const channel value);
};

struct texture::wrap : public texture::parameter {
	using parameter::parameter;
	struct s_type : texture::parameter {};
	struct t_type : texture::parameter {};
	struct r_type : texture::parameter {};

	enum class type { s, t, r };
	type current_type{ type::s };

	enum class mode {
		repeat,
		mirrored_repeat,
		clamp_to_edge,
		clamp_to_border,
		mirror_clamp_to_edge
	};
	wrap &operator[](const type new_type) noexcept;
	wrap &operator=(const mode value);
};


} // namespace golxzn::graphics::types
