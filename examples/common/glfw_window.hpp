#pragma once

#include <string_view>
#include <golxzn/common.hpp>
#include <golxzn/core/types.hpp>

namespace window{

static constexpr golxzn::core::u32 width{ 800 };
static constexpr golxzn::core::u32 height{ 800 };
static constexpr auto aspect{
	static_cast<golxzn::core::f16>(window::width) / static_cast<golxzn::core::f16>(window::height) };

inline GLFWwindow *initialize(const std::string_view title) {
	glfwSetErrorCallback([](int code, const char *desc) {
		spdlog::error("[GLFW] [{}] {}", code, desc);
	});

	if (glfwInit() == GLFW_FALSE) {
		spdlog::critical("Failed to initialize GLFW");
		return nullptr;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	auto window{ glfwCreateWindow(window::width, window::height, title.data(), nullptr, nullptr) };
	if (window == nullptr) {
		spdlog::critical("Failed to create GLFW window");
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow *, int width, int height) {
		glViewport(0, 0, width, height);
	});

	return window;
}

} // namespace window

