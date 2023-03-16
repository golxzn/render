#include "golxzn/common.hpp"

#include "golxzn/graphics/window/implementations/glfw_impl.hpp"

namespace golxzn::graphics {

bool glfw_impl::initialize(const core::u32 width, const core::u32 height, const std::string_view title) {
	if (!implementation::initialize(width, height, title)) {
		return false;
	}

	spdlog::info("[{}] Creating window", class_name);
	mWindow = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
	if (mWindow == nullptr) {
		const char *error{ nullptr };
		const auto error_id{ glfwGetError(&error) };
		spdlog::critical("[{}] Failed to create GLFW window ({}): {}", class_name, error_id, error);
		destroy();
		return false;
	}
	glfwMakeContextCurrent(mWindow);
	glfwSetFramebufferSizeCallback(mWindow, &glfw_impl::framebuffer_size_callback);
	return true;
}
void glfw_impl::destroy() {
	glfwDestroyWindow(mWindow);
}

bool glfw_impl::should_close() const noexcept {
	return glfwWindowShouldClose(mWindow);
}
void glfw_impl::swap_buffers() noexcept {
	glfwSwapBuffers(mWindow);
}

void glfw_impl::framebuffer_size_callback(GLFWwindow *, const int width, const int height) {
	if (auto api{ window::api() }; api) {
		api->resize(static_cast<core::u32>(width), static_cast<core::u32>(height));
	}
}


} // namespace golxzn::graphics

