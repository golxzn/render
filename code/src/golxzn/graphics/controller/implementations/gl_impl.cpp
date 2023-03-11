#include <spdlog/spdlog.h>

#include "golxzn/graphics/controller/implementations/gl_impl.hpp"

namespace golxzn::graphics {

/// @todo make this configurable
constexpr core::u32 SCR_WIDTH{ 800 };
constexpr core::u32 SCR_HEIGHT{ 600 };

void framebuffer_size_callback(GLFWwindow*, int width, int height) {
	glViewport(0, 0, width, height);
}

gl_impl::gl_impl() {
	spdlog::info("[{}]: Initializing OpenGL", class_name);
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	spdlog::info("[{}]: Creating window", class_name);
	// glfw window creation
	// --------------------
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr) {
		spdlog::critical("[{}]: Failed to create GLFW window.", class_name);
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	spdlog::info("[{}]: Loading OpenGL function pointers", class_name);
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
		spdlog::critical("[{}]: Failed to initialize GLAD.", class_name);
		glfwTerminate();
		return;
	}

	spdlog::info("[{}]: Initialization has finished", class_name);
}
gl_impl::~gl_impl() {
	glfwDestroyWindow(window);
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

bool gl_impl::window_should_close() const noexcept {
	return glfwWindowShouldClose(window);
}
void gl_impl::swap_window_buffers() noexcept {
	glfwSwapBuffers(window);
}



} // namespace golxzn::graphics
