#pragma once

#include <golxzn/graphics/controller/controller.hpp>

namespace golxzn::graphics {

class gl_impl final : public controller::implementation {
	static constexpr std::string_view class_name{ "graphics::gl_impl" };
public:
	~gl_impl() override = default;

	bool initialize() override;
	void destroy() override;

	ctrl::object::ref make_shader(const std::string_view type, const std::string_view code) override;
	ctrl::object::ref make_program() override;
	ctrl::object::ref make_texture(const std::string &path) override;

	void viewport(const core::u32 x, const core::u32 y,
		const core::u32 width, const core::u32 height) noexcept override;

};

} // namespace golxzn::graphics
