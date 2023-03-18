#include "stb_image.h"
#include "golxzn/common.hpp"

#include "golxzn/graphics/controller/implementations/gl_impl.hpp"
#include "golxzn/graphics/controller/opengl/VAO.hpp"
#include "golxzn/graphics/controller/opengl/VBO.hpp"
#include "golxzn/graphics/controller/opengl/EBO.hpp"
#include "golxzn/graphics/window/window.hpp"
#include "golxzn/graphics/types/shader_program.hpp"

void GLAPIENTRY
MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar* message, const void* userParam)
{
	static const golxzn::core::umap<GLenum, std::string_view> sources{
		{ GL_DEBUG_SOURCE_API, "API" },
		{ GL_DEBUG_SOURCE_WINDOW_SYSTEM, "window system" },
		{ GL_DEBUG_SOURCE_SHADER_COMPILER, "shader compiler" },
		{ GL_DEBUG_SOURCE_THIRD_PARTY, "third party" },
		{ GL_DEBUG_SOURCE_APPLICATION, "application" },
		{ GL_DEBUG_SOURCE_OTHER, "other" }
	};
	static const golxzn::core::umap<GLenum, std::string_view> severities{
		{ GL_DEBUG_SEVERITY_HIGH, "high" },
		{ GL_DEBUG_SEVERITY_MEDIUM, "medium" },
		{ GL_DEBUG_SEVERITY_LOW, "low" },
		{ GL_DEBUG_SEVERITY_NOTIFICATION, "notification" }
	};

	const auto str_severity{ severities.at(severity) };
	const auto str_source{ sources.at(source) };

	std::string msg{ "empty message" };
	if (message != nullptr) {
		msg = message;
	}

	if (type == GL_DEBUG_TYPE_ERROR) {
		spdlog::error("[GL] [{}] [{}] ID: {}; message: {}", str_severity, str_source, id, msg);
		return;
	}
	spdlog::info("[GL] [{}] [{}] ID: {}; message: {}", str_severity, str_source, id, msg);
}
namespace golxzn::graphics {

bool gl_impl::initialize() {
	spdlog::info("[{}] Initializing OpenGL", class_name);
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

	spdlog::info("[{}] Loading OpenGL function pointers", class_name);
	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
		spdlog::critical("[{}] Failed to initialize GLAD.", class_name);
		destroy();
		return false;
	}

	if (spdlog::get_level() == spdlog::level::debug) {
		// During init, enable debug output
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, nullptr);
	}

	spdlog::info("[{}] Initialization has finished", class_name);

	stbi_set_flip_vertically_on_load(true);

	GLint value{};
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value);
	max_texture_units = static_cast<core::u32>(value);

	return true;
}

void gl_impl::destroy() {
	glfwTerminate();
}

types::object::ref gl_impl::make_shader(const types::shader::type type, const std::string_view code) {
	using namespace types;

	if (const bool invalid_type{ type == shader::type::invalid }; invalid_type || code.empty()) {
		spdlog::error("[{}] Shader {}",
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
		auto obj{ types::object::make(glCreateShader(found->second), [](types::object &obj) {
			glDeleteShader(obj.id());
		} ) };

		obj->set_property<types::shader::type>("type", type);
		obj->set_property<GLenum>("gl_type", found->second);

		const auto code_cstr{ code.data() };
		const auto id{ obj->id() };
		glShaderSource(id, 1, &code_cstr, nullptr);
		glCompileShader(id);

		int success{};
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) {
			char info_log[512]{};
			GLsizei length{};
			glGetShaderInfoLog(id, info_log_size, &length, info_log);
			spdlog::error("[{}] Compilation error:\n {}", class_name,
				std::string{ info_log, info_log + length });
			return nullptr;
		}

		return obj;
	}

	spdlog::error("[{}] Shader type is invalid", class_name);
	return nullptr;
}
types::object::ref gl_impl::make_program() {
	return types::object::make(glCreateProgram(), [](types::object &obj) {
		glDeleteProgram(obj.id());
	});
}
types::object::ref gl_impl::make_texture() {
	core::u32 id{};
	glGenTextures(1, &id);

	return types::object::make(static_cast<types::object::id_t>(id), [](types::object &obj) {
		const auto id{ static_cast<GLuint>(obj.id()) };
		glDeleteTextures(1, &id);
	});
}

types::object::ref gl_impl::make_mesh(const std::vector<types::vertex> &vertices, const std::vector<core::u32> &indices) {
	if (vertices.empty()) {
		spdlog::error("[{}] Mesh cannot be empty", class_name);
		return nullptr;
	}

	core::u32 VAO;
	core::u32 VBO;
	core::u32 EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	if (!indices.empty()) {
		glGenBuffers(1, &EBO);
	}
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(types::vertex), vertices.data(), GL_STATIC_DRAW);

	if (!indices.empty()) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(core::u32), indices.data(), GL_STATIC_DRAW);
	}

	core::u32 attribute_pos{};
	const auto make_attribute = [&attribute_pos](const GLint size, const GLuint offset, const GLenum type = GL_FLOAT, const GLboolean normalized = GL_FALSE) {
		glVertexAttribPointer(attribute_pos, size, type, normalized, sizeof(types::vertex), reinterpret_cast<GLvoid *>(offset));
		glEnableVertexAttribArray(attribute_pos);
		++attribute_pos;
	};

	make_attribute(3, offsetof(types::vertex, position));
	make_attribute(3, offsetof(types::vertex, normal));
	make_attribute(2, offsetof(types::vertex, UV));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (!indices.empty()) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	auto obj{ types::object::make(VAO, [](types::object &obj) {
		if (const auto VBO{ obj.get_property<core::u32>("VBO") }; VBO.has_value()) {
			glDeleteBuffers(1, &VBO.value());
		}
		if (const auto EBO{ obj.get_property<core::u32>("EBO") }; EBO.has_value()) {
			glDeleteBuffers(1, &EBO.value());
		}

		const auto VAO{ obj.id() };
		glDeleteVertexArrays(1, &VAO);
	}) };

	obj->set_property<core::u32>("VAO", VAO);
	obj->set_property<core::u32>("VBO", VBO);
	obj->set_property<core::u32>("vertices_count", static_cast<core::u32>(vertices.size()));
	if (!indices.empty()) {
		obj->set_property<core::u32>("EBO", EBO);
		obj->set_property<core::u32>("indices_count", static_cast<core::u32>(indices.size()));
	}

	return obj;
}

bool gl_impl::attach_shader(const types::object::ref &program, const types::object::ref &shader) {
	if (!check_program_and_shader(program, shader)) {
		return false;
	}
	glAttachShader(program->id(), shader->id());
	return true;
}
bool gl_impl::detach_shader(const types::object::ref &program, const types::object::ref &shader) {
	if (!check_program_and_shader(program, shader)) {
		return false;
	}
	glDetachShader(program->id(), shader->id());
	return true;
}
bool gl_impl::link_program(const types::object::ref &program) {
	if (program == nullptr) {
		spdlog::error("[{}] Program is null", class_name);
		return false;
	}
	const auto id{ program->id() };
	if (glIsProgram(id) == GL_FALSE) {
		spdlog::error("[{}] {} is not a valid program", class_name, id);
		return false;
	}

	glLinkProgram(id);

	int success{};
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		std::string info_log(info_log_size, '\0');
		glGetProgramInfoLog(id, info_log_size, nullptr, &info_log[0]);
		spdlog::error("[{}] Cannot link program: \n{}", info_log);
		return false;
	}

	return true;
}
void gl_impl::use_program(const types::object::ref &program) {
	glUseProgram(program != nullptr ? program->id() : types::object::invalid_id);
}

void gl_impl::set_uniform(const types::object::ref &program, const std::string_view name,
	const std::any value, const std::type_info &info) {
	if (program == nullptr) {
		spdlog::error("[{}] Program is null", class_name);
		return;
	}

	const auto id{ program->id() };
	if (glIsProgram(id) == GL_FALSE) {
		spdlog::error("[{}] {} is not a valid program", class_name, id);
		return;
	}

	const auto location{ glGetUniformLocation(id, name.data()) };
	// if (location == -1) {
		// spdlog::warn("[{}] Cannot find uniform '{}' in the '{}' program",
		// 	class_name, name, program->get_property<std::string>("name").value_or("unnamed"));
		// return;
	// }
	if (info == typeid(core::f16)) {
		glUniform1f(location, std::any_cast<core::f16>(value));
	} else if (info == typeid(core::f32)) {
		glUniform1d(location, std::any_cast<core::f32>(value));
	} else if (info == typeid(core::i32)) {
		glUniform1i(location, std::any_cast<core::i32>(value));
	} else if (info == typeid(core::u32)) {
		glUniform1ui(location, std::any_cast<core::u32>(value));
	} else if (info == typeid(glm::vec3)) {
		auto vec{ std::any_cast<glm::vec3>(value) };
		glUniform3fv(location, 1, glm::value_ptr(vec));
	} else if (info == typeid(glm::mat4)) {
		auto mat{ std::any_cast<glm::mat4>(value) };
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	} else {
		spdlog::error("[{}] Invalid uniform type: {}", class_name, info.name());
	}
}

bool gl_impl::make_texture_image_2d(types::object::ref texture, const core::bytes &data) {
	if (texture == nullptr || !texture->valid()) {
		spdlog::error("[{}] Texture is null or invalid", class_name);
		return false;
	}
	if (data.empty()) {
		spdlog::error("[{}] Texture data is empty", class_name);
		return false;
	}
	using namespace golxzn::types_literals;

	core::i32 width{}, height{}, channels{};
	auto img{ stbi_load_from_memory(reinterpret_cast<const stbi_uc *>(data.data()), data.size(), &width, &height, &channels, 0) };
	if(img == nullptr) {
		spdlog::error("[{}] Cannot load texture image: {}", class_name, stbi_failure_reason());
		return false;
	}

	const auto translate_format{ [] (const auto channels) -> GLint {
		switch (channels) {
		case 1: return GL_RED;
		case 2: return GL_RG;
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
		default:
			spdlog::error("[{}] Invalid texture format: {}", class_name, channels);
			return GL_RGBA;
		}
	} };

	spdlog::debug("[{}] Texture #{} dimensions: {}x{}", class_name, texture->id(), width, height);

	texture->set_property("target", core::u32{ GL_TEXTURE_2D });
	texture->set_property("width", width);
	texture->set_property("height", height);

	bind_texture(texture, 0);

	const GLenum target{ GL_TEXTURE_2D };

	if (texture->get_property<bool>("setup_default_params").value_or(false)) {
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	const GLint format{ translate_format(channels) };
	glTexImage2D(target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, img);

	unbind_texture(texture);
	stbi_image_free(img);
	return true;
}

bool gl_impl::make_texture_image_2d(types::object::ref texture,
	const types::texture::cubemap_array<core::bytes> &data) {

	if (texture == nullptr || !texture->valid()) {
		spdlog::error("[{}] Texture is null or invalid", class_name);
		return false;
	}
	if (std::any_of(std::begin(data), std::end(data), [](const auto &d) { return d.empty(); })) {
		spdlog::error("[{}] Texture data is invalid", class_name);
		return false;
	}

	using namespace golxzn::types_literals;

	const auto translate_format{ [] (const auto channels) {
		switch (channels) {
		case 1: return GL_RED;
		case 2: return GL_RG;
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
		default:
			spdlog::error("[{}] Invalid texture format: {}", class_name, channels);
			return GL_RGBA;
		}
	} };

	texture->set_property("target", core::u32{ GL_TEXTURE_CUBE_MAP });

	bind_texture(texture, 0);

	const auto target{ GL_TEXTURE_CUBE_MAP };

	if (texture->get_property<bool>("setup_default_params").value_or(false)) {
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	static constexpr core::u32 targets_size{ 6 };
	static constexpr GLenum targets[targets_size]{
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
	};

	for (core::u32 id{}; id < targets_size; ++id) {
		core::i32 width{}, height{}, channels{};
		auto img{ stbi_load_from_memory(
			reinterpret_cast<const stbi_uc *>(data[id].data()),
			data[id].size(), &width, &height, &channels, 0)
		};
		if(img == nullptr) {
			spdlog::error("[{}] Cannot load texture image: {}", class_name, stbi_failure_reason());
			break;
		}

		const GLint format{ translate_format(channels) };
		glTexImage2D(targets[id], 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, img);
		stbi_image_free(img);
	}

	unbind_texture(texture);
	return true;
}

void gl_impl::generate_mip_maps(const types::object::ref &texture) {
	if (texture == nullptr) {
		spdlog::error("[{}] Texture is null", class_name);
		return;
	}

	using namespace types;
	const auto type{ texture->get_property<texture::type>(texture::param_type)
		.value_or(texture::type::invalid) };

	if (type == texture::type::invalid) {
		return;
	}

	bind_texture(texture, 0);

	const auto target{ texture->get_property<core::u32>("target").value_or(GL_TEXTURE_2D) };
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(static_cast<GLenum>(target));

	unbind_texture(texture);
}

bool gl_impl::bind_texture(const types::object::ref &texture, const core::u32 unit) {
	if (texture == nullptr || !texture->valid()) {
		return false;
	}
	if (unit > max_texture_units) {
		const auto tex_name{ texture->get_property<std::string>("name").value_or("unknown") };
		spdlog::error("[{}] Tried to bind the '{}' texture to the {} unit, but there are only {} units",
			class_name, tex_name, unit, max_texture_units);
		return false;
	} else {
		glActiveTexture(GL_TEXTURE0 + unit);
	}

	const auto id{ static_cast<GLuint>(texture->id()) };
	const auto target{ texture->get_property<core::u32>("target").value_or(GL_TEXTURE_2D) };
	glBindTexture(static_cast<GLenum>(target), id);

	return true;
}

bool gl_impl::unbind_texture(const types::object::ref &texture) {
	if (texture == nullptr || !texture->valid()) {
		return false;
	}

	const auto target{ texture->get_property<core::u32>("target").value_or(GL_TEXTURE_2D) };
	glBindTexture(static_cast<GLenum>(target), GLuint{ 0 });

	return true;
}

void gl_impl::set_raw_texture_parameter(const types::object::ref &texture,
	const std::type_info &param_type, const std::any param_value) {
	if (texture == nullptr || !texture->valid()) {
		return;
	}

	using namespace types;

	bind_texture(texture, 0);

	const auto target{ texture->get_property<core::u32>("target").value_or(GL_TEXTURE_2D) };

	if (param_type == typeid(texture::depth_stencil_texture_mode)) {
		set_texture_depth_stencil_mode(target, param_value);
	} else if (param_type == typeid(texture::base_level)) {
		set_texture_base_level(target, param_value);
	} else if (param_type == typeid(texture::compare_function)) {
		set_texture_compare_function(target, param_value);
	} else if (param_type == typeid(texture::compare_mode)) {
		set_texture_compare_mode(target, param_value);
	} else if (param_type == typeid(texture::min_filter)) {
		set_texture_min_filter(target, param_value);
	} else if (param_type == typeid(texture::mag_filter)) {
		set_texture_mag_filter(target, param_value);
	} else if (param_type == typeid(texture::lod::min_level)) {
		set_texture_lod(target, texture::lod::level::min, param_value);
	} else if (param_type == typeid(texture::lod::max_level)) {
		set_texture_lod(target, texture::lod::level::max, param_value);
	} else if (param_type == typeid(texture::lod::bias)) {
		set_texture_lod(target, texture::lod::level::bias, param_value);
	} else if (param_type == typeid(texture::swizzle::red_type)) {
		set_texture_swizzle(target, texture::swizzle::type::red, param_value);
	} else if (param_type == typeid(texture::swizzle::green_type)) {
		set_texture_swizzle(target, texture::swizzle::type::green, param_value);
	} else if (param_type == typeid(texture::swizzle::blue_type)) {
		set_texture_swizzle(target, texture::swizzle::type::blue, param_value);
	} else if (param_type == typeid(texture::swizzle::alpha_type)) {
		set_texture_swizzle(target, texture::swizzle::type::alpha, param_value);
	} else if (param_type == typeid(texture::wrap::s_type)) {
		set_texture_wrap(target, texture::wrap::type::s, param_value);
	} else if (param_type == typeid(texture::wrap::t_type)) {
		set_texture_wrap(target, texture::wrap::type::t, param_value);
	} else if (param_type == typeid(texture::wrap::r_type)) {
		set_texture_wrap(target, texture::wrap::type::r, param_value);
	} else {
		spdlog::error("[{}] Unhandled parameter type: {}", class_name, param_type.name());
	}

	unbind_texture(texture);
}


void gl_impl::draw_mesh(const types::object::ref &mesh) {
	if (mesh == nullptr || !mesh->valid()) {
		spdlog::warn("[{}] Failed to draw an invalid mesh", class_name);
		return;
	}

	const auto VAO{ mesh->get_property<core::u32>("VAO") };
	if (!VAO.has_value()) {
		const auto name{ mesh->get_property<std::string>("name").value_or("unknown") };
		spdlog::error("[{}] Failed to draw the '{}' mesh, the VAO is not set", name, class_name);
	}

	const auto depth_test{ mesh->get_property<bool>("depth_test").value_or(false) };
	if (depth_test) {
		glEnable(GL_DEPTH_TEST);
	}

	glBindVertexArray(VAO.value());

	if (const auto indices_count{ mesh->get_property<core::u32>("indices_count") }; indices_count.has_value()) {
		glDrawElements(GL_TRIANGLES, indices_count.value(), GL_UNSIGNED_INT, 0);
	} else {
		const auto vertices_count{ mesh->get_property<core::u32>("vertices_count") };
		glDrawArrays(GL_TRIANGLES, 0, vertices_count.value());
	}

	glBindVertexArray(0);

	if (depth_test) {
		glDisable(GL_DEPTH_TEST);
	}
}

void gl_impl::viewport(const core::u32 x, const core::u32 y, const core::u32 width, const core::u32 height) noexcept {
	glViewport(x, y, width, height);
}

bool gl_impl::check_program_and_shader(const types::object::ref &program, const types::object::ref &shader) const {
	if (program == nullptr || shader == nullptr) {
		if (program == nullptr && shader == nullptr) {
			spdlog::error("[{}] Program and shader are null", class_name);
		} else {
			spdlog::error("[{}] {}", class_name,
				(program == nullptr ? "program is null" : ""),
				(shader == nullptr ? "shader is null" : "")
			);
		}
		return false;
	}

	if (glIsProgram(program->id()) == GL_FALSE) {
		spdlog::error("[{}] {} is not a valid program", class_name, program->id());
		return false;
	}
	if (glIsShader(shader->id()) == GL_FALSE) {
		spdlog::error("[{}] {} is not a valid shader", class_name, program->id());
		return false;
	}
	return true;
}

void gl_impl::set_texture_depth_stencil_mode(const core::u32 target, const std::any mode) const {
	using depth_stencil_mode = types::texture::depth_stencil_texture_mode::mode;

	const auto *value_ptr{ std::any_cast<depth_stencil_mode>(&mode) };
	if (value_ptr == nullptr) {
		spdlog::error("[{}] Cannot get depth_stencil_texture_mode value", class_name);
		return;
	}

	static const core::umap<depth_stencil_mode, core::i32> mode_map{
		{ depth_stencil_mode::stencil_index,   GL_STENCIL_INDEX    },
		{ depth_stencil_mode::depth_component, GL_DEPTH_COMPONENT  },
		{ depth_stencil_mode::depth_stencil,   GL_DEPTH_STENCIL    },
	};

	glTexParameteri(target, GL_DEPTH_STENCIL_TEXTURE_MODE, mode_map.at(*value_ptr));
}

void gl_impl::set_texture_base_level(const core::u32 target, const std::any level) const {
	if (const auto *value_ptr{ std::any_cast<core::u32>(&level) }; value_ptr != nullptr) {
		glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, *value_ptr);
		return;
	}
	spdlog::error("[{}] Cannot get base level value", class_name);
}
void gl_impl::set_texture_compare_function(const core::u32 target, const std::any function) const {
	using func = types::texture::compare_function::function;

	const auto *value_ptr{ std::any_cast<func>(&function) };
	if (value_ptr == nullptr) {
		spdlog::error("[{}] Cannot get compare function value", class_name);
		return;
	}

	static const core::umap<func, GLenum> lookup{
		{ func::never,    GL_NEVER    },
		{ func::less,     GL_LESS     },
		{ func::equal,    GL_EQUAL    },
		{ func::lequal,   GL_LEQUAL   },
		{ func::greater,  GL_GREATER  },
		{ func::notequal, GL_NOTEQUAL },
		{ func::gequal,   GL_GEQUAL   },
		{ func::always,   GL_ALWAYS   },
	};

	glTexParameteri(target, GL_TEXTURE_COMPARE_FUNC, lookup.at(*value_ptr));
}
void gl_impl::set_texture_compare_mode(const core::u32 target, const std::any mode) const {
	using comp_mode = types::texture::compare_mode::mode;

	if (const auto *value_ptr{ std::any_cast<comp_mode>(&mode) }; value_ptr != nullptr) {
		const auto value{ *value_ptr == comp_mode::none ? GL_NONE : GL_COMPARE_REF_TO_TEXTURE };
		glTexParameteri(target, GL_TEXTURE_COMPARE_MODE, value);
		return;
	}
	spdlog::error("[{}] Cannot get compare mode value", class_name);
}
void gl_impl::set_texture_min_filter(const core::u32 target, const std::any filter) const {
	using min_filter = types::texture::min_filter::filter;

	const auto *value_ptr{ std::any_cast<min_filter>(&filter) };
	if (value_ptr == nullptr) {
		spdlog::error("[{}] Cannot get min_filter value", class_name);
		return;
	}

	static const core::umap<min_filter, core::i32> lookup{
		{ min_filter::nearest,                  GL_NEAREST                },
		{ min_filter::linear,                   GL_LINEAR                 },
		{ min_filter::nearest_mipmap_nearest,   GL_NEAREST_MIPMAP_NEAREST },
		{ min_filter::linear_mipmap_nearest,    GL_LINEAR_MIPMAP_NEAREST  },
		{ min_filter::nearest_mipmap_linear,    GL_NEAREST_MIPMAP_LINEAR  },
		{ min_filter::linear_mipmap_linear,     GL_LINEAR_MIPMAP_LINEAR   },
	};
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, lookup.at(*value_ptr));
}
void gl_impl::set_texture_mag_filter(const core::u32 target, const std::any filter) const {
	using mag_filter = types::texture::mag_filter::filter;

	const auto *value_ptr{ std::any_cast<mag_filter>(&filter) };
	if (value_ptr == nullptr) {
		spdlog::error("[{}] Cannot get min_filter value", class_name);
		return;
	}

	static const core::umap<mag_filter, core::i32> lookup{
		{ mag_filter::nearest,    GL_NEAREST    },
		{ mag_filter::linear,     GL_LINEAR     },
	};
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, lookup.at(*value_ptr));
}
void gl_impl::set_texture_lod(const core::u32 target, const types::texture::lod::level level, const std::any value) const {
	using lod_lvl = types::texture::lod::level;

	const auto *value_ptr{ std::any_cast<core::f32>(&value) };
	if (value_ptr == nullptr) {
		spdlog::error("[{}] Cannot get lod value", class_name);
		return;
	}
	static const core::umap<lod_lvl, core::u32> level_map{
		{ lod_lvl::min,  GL_TEXTURE_MIN_LOD },
		{ lod_lvl::max,  GL_TEXTURE_MAX_LOD },
		{ lod_lvl::bias, GL_TEXTURE_LOD_BIAS },
	};
	glTexParameterf(target, level_map.at(level), static_cast<GLfloat>(*value_ptr));
}

void gl_impl::set_texture_swizzle(const core::u32 target, const types::texture::swizzle::type type, const std::any channel) const {
	using swizzle_type = types::texture::swizzle::type;
	using swizzle_channel = types::texture::swizzle::channel;

	const auto *value_ptr{ std::any_cast<swizzle_channel>(&channel) };
	if (value_ptr == nullptr) {
		spdlog::error("[{}] Cannot get swizzle channel", class_name);
		return;
	}

	static const core::umap<swizzle_channel, GLenum> channel_lookup{
		{ swizzle_channel::red,   GL_RED   },
		{ swizzle_channel::green, GL_GREEN },
		{ swizzle_channel::blue,  GL_BLUE  },
		{ swizzle_channel::alpha, GL_ALPHA },
		{ swizzle_channel::zero,  GL_ZERO  },
		{ swizzle_channel::one,   GL_ONE   },
	};
	static const core::umap<swizzle_type, core::u32> mode_lookup{
		{ swizzle_type::red,   GL_TEXTURE_SWIZZLE_R },
		{ swizzle_type::green, GL_TEXTURE_SWIZZLE_G },
		{ swizzle_type::blue,  GL_TEXTURE_SWIZZLE_B },
		{ swizzle_type::alpha, GL_TEXTURE_SWIZZLE_A },
	};
	glTexParameteri(target, mode_lookup.at(type), channel_lookup.at(*value_ptr));
}

void gl_impl::set_texture_wrap(const core::u32 target, const types::texture::wrap::type type, const std::any wrap) const {
	using wrap_type = types::texture::wrap::type;
	using wrap_mode = types::texture::wrap::mode;


	const auto *value_ptr{ std::any_cast<wrap_mode>(&wrap) };
	if (value_ptr == nullptr) {
		spdlog::error("[{}] Cannot get wraping mode", class_name);
		return;
	}

	static const core::umap<wrap_type, core::u32> type_lookup{
		{ wrap_type::s, GL_TEXTURE_WRAP_S },
		{ wrap_type::t, GL_TEXTURE_WRAP_T },
		{ wrap_type::r, GL_TEXTURE_WRAP_R },
	};
	static const core::umap<wrap_mode, core::i32> mode_lookup{
		{ wrap_mode::clamp_to_edge,         GL_CLAMP_TO_EDGE         },
		{ wrap_mode::clamp_to_border,       GL_CLAMP_TO_BORDER       },
		{ wrap_mode::mirrored_repeat,       GL_MIRRORED_REPEAT       },
		{ wrap_mode::repeat,                GL_REPEAT                },
		{ wrap_mode::mirrored_repeat,       GL_MIRRORED_REPEAT       },
		{ wrap_mode::mirror_clamp_to_edge,  GL_MIRROR_CLAMP_TO_EDGE  },
	};
	glTexParameteri(target, type_lookup.at(type), mode_lookup.at(*value_ptr));
}

} // namespace golxzn::graphics
