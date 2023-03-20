#pragma once

#include <golxzn/graphics/controller/controller.hpp>

namespace golxzn::graphics {

class gl_impl final : public controller::implementation {
	static constexpr std::string_view class_name{ "graphics::gl_impl" };
	static constexpr size_t info_log_size{ 512 };
public:
	~gl_impl() override = default;

	bool initialize() override;
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

	bool make_texture_image_2d(types::object::ref texture, const core::bytes &data) override;

	bool make_texture_image_2d(types::object::ref texture,
		const types::texture::cubemap_array<core::bytes> &data) override;

	void generate_mip_maps(const types::object::ref &texture) override;

	bool bind_texture(const types::object::ref &texture, const core::u32 unit) override;
	bool unbind_texture(const types::object::ref &texture) override;

	void set_raw_texture_parameter(const types::object::ref &program,
		const std::type_info &param_type, const std::any param_value) override;

	void draw_mesh(const types::object::ref &mesh) override;

	void viewport(const core::u32 x, const core::u32 y,
		const core::u32 width, const core::u32 height) noexcept override;

	void enable(const capabilities capability) override;
	void enable(const capabilities capability, const core::u32 value) override;
	void disable(const capabilities capability) override;
	void disable(const capabilities capability, const core::u32 value) override;
	bool is_enabled(const capabilities capability) const override;
	bool is_enabled(const capabilities capability, const core::u32 value) const override;
	core::i32 capability_value(const capabilities capability) const override;

private:
	core::u32 max_texture_units{ 0 };
	static const core::umap<capabilities, core::u32> gl_capability_map;

	bool check_program_and_shader(const types::object::ref &program, const types::object::ref &shader) const;

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
