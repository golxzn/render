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

ctrl::object::ref gl_impl::make_shader(const types::shader::type type, const std::string_view code) {
	using namespace types;

	if (const bool invalid_type{ type == shader::type::invalid }; invalid_type || code.empty()) {
		spdlog::error("[{}]: Shader {}",
			class_name, (invalid_type ? "type is invalid" : "code cannot be empty."));
		return nullptr;
	}

	static const core::umap<shader::type, GLenum> type_map{
		{ shader::type::vertex, GL_VERTEX_SHADER },
		{ shader::type::tesselation_control, GL_TESS_CONTROL_SHADER },
		{ shader::type::tesselation_evaluation, GL_TESS_EVALUATION_SHADER },
		{ shader::type::geometry, GL_GEOMETRY_SHADER },
		{ shader::type::fragment, GL_FRAGMENT_SHADER },
		{ shader::type::compute, GL_COMPUTE_SHADER },
	};

	if (const auto found{ type_map.find(type) }; found != std::end(type_map)) {
		auto obj{ std::make_shared<ctrl::object>(
			static_cast<core::u32>(glCreateShader(found->second))) };

		obj->set_deleter([](ctrl::object &obj) {
			glDeleteShader(obj.id()); // could cause a crash. :thonk: about it.
		});
		obj->set_property<types::shader::type>("type", type);
		obj->set_property<GLenum>("gl_type", found->second);

		const auto code_cstr{ code.data() };
		const auto id{ obj->id() };
		glShaderSource(id, 1, &code_cstr, nullptr);
		glCompileShader(id);

		int success{};
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success) {
			static constexpr size_t buffer_size{ 512 };
			std::string info_log(buffer_size, '\0');
			glGetShaderInfoLog(id, buffer_size, nullptr, &info_log[0]);
			spdlog::error("[{}] Compilation error:\n {}", info_log);
			return nullptr;
		}

		return obj;
	}

	spdlog::error("[{}]: Shader type is invalid", class_name);
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
