#pragma once

#include "golxzn/graphics/window/window.hpp"

class GLFWwindow;

namespace golxzn::graphics {

class glfw_impl final : public window::implementation {
	static constexpr std::string_view class_name{ "graphics::glfw_impl" };
public:
	bool initialize(const core::u32 width, const core::u32 height,
		const std::string_view title) override;
	void destroy() override;

	bool should_close() const noexcept override;
	void swap_buffers() noexcept override;

	void resize(const core::u32 width, const core::u32 height,
			const bool update_viewport = true) noexcept override;
private:
	GLFWwindow *mWindow{ nullptr };

	static void framebuffer_size_callback(GLFWwindow *window, const int width, const int height);
};

} // namespace golxzn::graphics
