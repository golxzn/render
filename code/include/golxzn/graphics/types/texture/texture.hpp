#pragma once

#include <array>
#include <glm/fwd.hpp>
#include <golxzn/core/types/color.hpp>
#include <golxzn/core/types/image.hpp>

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
	static constexpr std::string_view param_bytes_count{ "bytes_count" };

	using ref = core::sptr<texture>;
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

	enum class target;
	enum class internal_format;
	enum class pixel_data_format;

	template<class T, std::enable_if_t<std::is_base_of_v<parameter, T>, bool> = false>
	T param() const noexcept { return T{ mObject }; }

	enum class type {
		invalid,
		texture_1d,
		texture_2d,
		texture_3d,
		texture_gif,
		cube_map
	};

	static ref make(type tex_type, const std::string &path);

	explicit texture(const std::string &name, const type texture_type = type::texture_2d);

	void bind(const core::u32 unit = 0) const noexcept;
	void unbind() const noexcept;

	type get_type() const noexcept;
	bool valid() const noexcept;
	object::id_t id() const noexcept;
	core::usize bytes_count() const noexcept;
	core::u32 width() const noexcept;
	core::u32 height() const noexcept;

	const std::vector<core::i32> &delays() const noexcept;

	void set_path(const std::string &path) noexcept;
	void set_bytes_count(const core::usize bytes_count) noexcept;

	void set_image(const target &target, const core::types::image::ref &img);
	void set_image(const target &target, const core::types::image::ref &img, const internal_format format);

	// void set_data(const target &target, const core::i32 size, const internal_format format,
	// 	const pixel_data_format data_format, const core::bytes &data);
	// void set_data(const target &target, const glm::i32vec2 size, const internal_format format,
	// 	const pixel_data_format data_format, const core::bytes &data);
	// void set_data(const target &target, const glm::i32vec3 size, const internal_format format,
	// 	const pixel_data_format data_format, const core::bytes &data);

	// void set_delays(const std::vector<core::i32> &delays);
	// void set_delays(std::vector<core::i32> &&delays);

	bool generate(const bool setup_default_params = true);
	bool generate_mip_maps();

	// void set_border_color(const core::color &color) noexcept;

	static pixel_data_format get_pixel_data_format(const core::types::image::ref &img) noexcept;
	static internal_format get_internal_format(const core::types::image::ref &img) noexcept;

private:
	object::ref mObject;
	std::vector<core::i32> mDelays;

	bool make_texture(const type tex_type);
};

enum class texture::target {
	// Texture 1D
	texture_1d, proxy_texture_1d,

	// Texture 2D
	texture_2d,        proxy_texture_2d,
	texture_1d_array,  proxy_texture_1d_array,
	texture_rectangle, proxy_texture_rectangle,
	texture_cube_map_positive_x, texture_cube_map_negative_x,
	texture_cube_map_positive_y, texture_cube_map_negative_y,
	texture_cube_map_positive_z, texture_cube_map_negative_z,
	proxy_texture_cube_map,

	// Texture 3D
	texture_3d,       proxy_texture_3d,
	texture_2d_array, proxy_texture_2d_array,
};

enum class texture::internal_format {
	RGBA_32f, RGBA_32i, RGBA_32ui,
	RGBA_16, RGBA_16f, RGBA_16i, RGBA_16ui, RGBA_16_snorm,
	RGBA_8, RGBA_8i, RGBA_8ui, RGBA_8_snorm,
	SRGB_8_alpha8, SRGB_8,

	RGB_32f, RGB_32i, RGB_32ui,
	RGB_16, RGB_16f, RGB_16i, RGB_16ui, RGB_16_snorm,
	RGB_8, RGB_8i, RGB_8ui, RGB_8_snorm,

	RGB_10_a2, RGB_10_a2ui, RGB_9_e5,

	RG_32f, RG_32i, RG_32ui,
	RG_16, RG_16f, RG_16_snorm,
	RG_8, RG_8i, RG_8ui, RG_8_snorm,

	R11f_G11f_B10f,

	R_32f, R_32i, R_32ui,
	R_16f, R_16i, R_16ui, R_16_snorm,
	R_8, R_8i, R_8ui, R_8_snorm,

	compressed_red_RGTC1, compressed_signed_red_RGTC1,
	compressed_RG_RGTC2, compressed_signed_RG_RGTC2,

	depth_component_32f, depth_component_24, depth_component_16,
	depth_32f_stencil_8, depth_24_stencil_8,
};

enum class texture::pixel_data_format {
	RED,
	RG,
	RGB,
	BGR,
	RGBA,
	BGRA,
};

using tex_type = texture::type;
using tex_target = texture::target;
using tex_format = texture::internal_format;
using tex_data_format = texture::pixel_data_format;

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
