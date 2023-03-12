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

	ctrl::object::ref make_shader(const types::shader::type type, const std::string_view code) override;
	ctrl::object::ref make_program() override;
	ctrl::object::ref make_texture(const std::string &path) override;

	bool attach_shader(const ctrl::object::ref &program, const ctrl::object::ref &shader) override;
	bool detach_shader(const ctrl::object::ref &program, const ctrl::object::ref &shader) override;
	bool link_program(const ctrl::object::ref &program) override;
	void use_program(const ctrl::object::ref &program) override;

	void viewport(const core::u32 x, const core::u32 y,
		const core::u32 width, const core::u32 height) noexcept override;

private:

	bool check_program_and_shader(const ctrl::object::ref &program, const ctrl::object::ref &shader) const;
};

} // namespace golxzn::graphics
