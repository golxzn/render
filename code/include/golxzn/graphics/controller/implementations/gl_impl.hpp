#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <golxzn/graphics/controller/controller.hpp>

namespace golxzn::graphics {

class gl_impl final : public controller::implementation {
	static constexpr std::string_view class_name{ "graphics::gl_impl" };
public:
	gl_impl();
	~gl_impl() override;

	ctrl::object::ref make_shader(const std::string_view type, const std::string_view code) override;
	ctrl::object::ref make_program() override;
	ctrl::object::ref make_texture(const std::string &path) override;

	/// @todo move the window stuff outside
	bool window_should_close() const noexcept override;
	void swap_window_buffers() noexcept override;

private:
	GLFWwindow *window{ nullptr };
};

} // namespace golxzn::graphics
