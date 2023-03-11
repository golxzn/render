#include "golxzn/common.hpp"

#include "golxzn/graphics/controller/implementations/gl_impl.hpp"

#include <golxzn/graphics/window/window.hpp>

namespace golxzn::graphics {

bool gl_impl::initialize() {
	spdlog::info("[{}]: Initializing OpenGL", class_name);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	if (!window::initialize()) {
		return false;
	}

	spdlog::info("[{}]: Loading OpenGL function pointers", class_name);
	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
		spdlog::critical("[{}]: Failed to initialize GLAD.", class_name);
		destroy();
		return false;
	}

	spdlog::info("[{}]: Initialization has finished", class_name);
	return true;
}

void gl_impl::destroy() {
	glfwTerminate();
}

ctrl::object::ref gl_impl::make_shader(const std::string_view type, const std::string_view code) {
	if (type.empty() || code.empty()) {
		spdlog::error("[{}]: Shader {} cannot be empty.",
			class_name, (type.empty() ? "type" : "code"));
		return nullptr;
	}


	return nullptr;
}
ctrl::object::ref gl_impl::make_program() {
	return nullptr;
}
ctrl::object::ref gl_impl::make_texture(const std::string &path) {
	return nullptr;
}

void gl_impl::viewport(const core::u32 x, const core::u32 y, const core::u32 width, const core::u32 height) noexcept {
	glViewport(x, y, width, height);
}

} // namespace golxzn::graphics
