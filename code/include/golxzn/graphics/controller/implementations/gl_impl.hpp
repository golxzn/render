#pragma once

#include <golxzn/graphics/controller/controller.hpp>

namespace golxzn::graphics {

class gl_impl final : public controller::implementation {
	static constexpr std::string_view class_name{ "graphics::gl_impl" };
	static constexpr size_t info_log_size{ 512 };
public:
	~gl_impl() override = default;

	bool initialize(controller::get_process_address_function function) override;
	void destroy() override;

	types::object::ref make_shader(const types::shader::type type, const std::string_view code) override;
	types::object::ref make_program() override;
	types::object::ref make_texture() override;
	types::object::ref make_mesh(const std::vector<types::vertex> &vertices, const std::vector<core::u32> &indices) override;

	bool attach_shader(const types::object::ref &program, const types::object::ref &shader) override;
	bool detach_shader(const types::object::ref &program, const types::object::ref &shader) override;
	bool link_program(const types::object::ref &program) override;
	void use_program(const types::object::ref &program) override;

	void set_uniform(const types::object::ref &program, const std::string_view name,
		const std::any value, const std::type_info &info) override;

	void set_texture_image_ext(types::object::ref texture,
		const types::tex_target target, const core::i32 size,
		const types::tex_format internal_format, const types::tex_data_format data_format,
		const core::u8 *data) override;

	void set_texture_image_ext(types::object::ref texture,
		const types::texture::target target, const glm::i32vec2 &size,
		const types::tex_format internal_format, const types::tex_data_format data_format,
		const core::u8 *data) override;

	void set_texture_image_ext(types::object::ref texture,
		const types::texture::target target, const glm::i32vec3 &size,
		const types::tex_format internal_format, const types::tex_data_format data_format,
		const core::u8 *data) override;

	void set_texture_image(types::object::ref texture, const core::i32 width,
		const core::i32 height, const core::i32 channels, const core::u8 *data) override;


	bool make_texture_image_2d(types::object::ref texture, const core::bytes &data) override;

	bool make_texture_image_2d(types::object::ref texture, const types::texture::cubemap_array<core::bytes> &data) override;


	void generate_mip_maps(const types::object::ref &texture) override;

	bool bind_texture(const types::object::ref &texture, const core::u32 unit) override;
	bool unbind_texture(const types::object::ref &texture) override;

	void set_raw_texture_parameter(const types::object::ref &program,
		const std::type_info &param_type, const std::any param_value) override;

	void draw_mesh(const types::object::ref &mesh) override;

	void viewport(const core::u32 x, const core::u32 y,
		const core::u32 width, const core::u32 height) noexcept override;

	void enable(const mods::capabilities capability) override;
	void enable(const mods::capabilities capability, const core::u32 value) override;
	void disable(const mods::capabilities capability) override;
	void disable(const mods::capabilities capability, const core::u32 value) override;
	bool is_enabled(const mods::capabilities capability) const override;
	bool is_enabled(const mods::capabilities capability, const core::u32 value) const override;
	core::i32 capability_value(const mods::capabilities capability) const override;

	void set_blend_color(const glm::vec4 &color) override;
	glm::vec4 get_blend_color() override;

	void set_blend_function(const mods::blend::function src, const mods::blend::function dest) override;
	std::pair<mods::blend::function, mods::blend::function> get_blend_function() override;

	void set_blend_equation(const mods::blend::equation equation) override;
	mods::blend::equation get_blend_equation() override;

	void set_blend_function_separate(
			const mods::blend::function src_rgb,
			const mods::blend::function dest_rgb,
			const mods::blend::function src_alpha,
			const mods::blend::function dest_alpha) override;
	std::array<mods::blend::function, 4> get_blend_function_separate() override;

	void set_blend_equation_separate(
			const mods::blend::equation equation_rgb,
			const mods::blend::equation equation_alpha) override;
	std::pair<mods::blend::equation, mods::blend::equation> get_blend_equation_separate() override;

	void set_depth_mask(const bool value) override;
	bool get_depth_mask() const override;

	void set_depth_function(const mods::depth::function func) override;
	mods::depth::function get_depth_function() const override;

	void set_depth_range(const core::f16 min, const core::f16 max) override;
	std::pair<core::f16, core::f16> get_depth_range() const override;

private:
	core::u32 max_texture_units{ 0 };
	static const core::umap<types::tex_target, core::u32> gl_tex_target_map;
	static const core::umap<types::tex_format, core::u32> gl_tex_format_map;
	static const core::umap<types::tex_data_format, core::u32> gl_tex_data_format_map;

	static const core::umap<mods::capabilities, core::u32> gl_capability_map;
	static const core::umap<mods::blend::function, core::u32> gl_blend_function_map;
	static const core::umap<mods::blend::equation, core::u32> gl_blend_equation_map;
	static const core::umap<mods::depth::function, core::u32> gl_depth_function_map;

	bool check_program_and_shader(const types::object::ref &program, const types::object::ref &shader) const;
	bool setup_texture(types::object::ref texture, const types::texture::target target,
		const types::tex_format internal_format, const types::tex_data_format data_format) const;
	core::i32 translate_texture_channel(const core::i32 channel) const noexcept;

	void set_texture_depth_stencil_mode(const core::u32 target, const std::any mode) const;
	void set_texture_base_level(const core::u32 target, const std::any level) const;
	void set_texture_compare_function(const core::u32 target, const std::any function) const;
	void set_texture_compare_mode(const core::u32 target, const std::any mode) const;
	void set_texture_min_filter(const core::u32 target, const std::any filter) const;
	void set_texture_mag_filter(const core::u32 target, const std::any filter) const;
	void set_texture_lod(const core::u32 target, const types::texture::lod::level level, const std::any value) const;
	void set_texture_swizzle(const core::u32 target, const types::texture::swizzle::type type, const std::any channel) const;
	void set_texture_wrap(const core::u32 target, const types::texture::wrap::type type, const std::any wrap) const;
};

} // namespace golxzn::graphics
