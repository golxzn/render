#include "golxzn/common.hpp"

#include "golxzn/graphics/controller/implementations/gl_impl.hpp"

#include "golxzn/graphics/mods/capabilities.hpp"
#include "golxzn/graphics/mods/blend.hpp"
#include "golxzn/graphics/mods/depth.hpp"

#include "golxzn/graphics/types/shader/program.hpp"

namespace golxzn::graphics {

const core::umap<types::tex_type, core::u32> gl_impl::gl_tex_type_map{
	{ types::tex_type::invalid,            core::u32{ GL_NONE } },
	{ types::tex_type::texture_1d,         core::u32{ GL_TEXTURE_1D } },
	{ types::tex_type::texture_2d,         core::u32{ GL_TEXTURE_2D } },
	{ types::tex_type::texture_3d,         core::u32{ GL_TEXTURE_3D } },
	{ types::tex_type::texture_gif,        core::u32{ GL_TEXTURE_2D_ARRAY } },
	{ types::tex_type::cube_map,           core::u32{ GL_TEXTURE_CUBE_MAP } },
};

const core::umap<types::tex_target, core::u32> gl_impl::gl_tex_target_map{
	{ types::tex_target::texture_2d,                   core::u32{ GL_TEXTURE_2D }                  },
	{ types::tex_target::proxy_texture_2d,             core::u32{ GL_PROXY_TEXTURE_2D }            },
	{ types::tex_target::texture_1d_array,             core::u32{ GL_TEXTURE_1D_ARRAY }            },
	{ types::tex_target::proxy_texture_1d_array,       core::u32{ GL_PROXY_TEXTURE_1D_ARRAY }      },
	{ types::tex_target::texture_rectangle,            core::u32{ GL_TEXTURE_RECTANGLE }           },
	{ types::tex_target::proxy_texture_rectangle,      core::u32{ GL_PROXY_TEXTURE_RECTANGLE }     },
	{ types::tex_target::texture_cube_map_positive_x,  core::u32{ GL_TEXTURE_CUBE_MAP_POSITIVE_X } },
	{ types::tex_target::texture_cube_map_negative_x,  core::u32{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X } },
	{ types::tex_target::texture_cube_map_positive_y,  core::u32{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y } },
	{ types::tex_target::texture_cube_map_negative_y,  core::u32{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y } },
	{ types::tex_target::texture_cube_map_positive_z,  core::u32{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z } },
	{ types::tex_target::texture_cube_map_negative_z,  core::u32{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z } },
	{ types::tex_target::proxy_texture_cube_map,       core::u32{ GL_PROXY_TEXTURE_CUBE_MAP }      },
};

const core::umap<types::tex_format, core::u32> gl_impl::gl_tex_format_map{
	{ types::tex_format::RGBA_32f,                     core::u32{ GL_RGBA32F }                     },
	{ types::tex_format::RGBA_32i,                     core::u32{ GL_RGBA32I }                     },
	{ types::tex_format::RGBA_32ui,                    core::u32{ GL_RGBA32UI }                    },
	{ types::tex_format::RGBA_16,                      core::u32{ GL_RGBA16 }                      },
	{ types::tex_format::RGBA_16f,                     core::u32{ GL_RGBA16F }                     },
	{ types::tex_format::RGBA_16i,                     core::u32{ GL_RGBA16I }                     },
	{ types::tex_format::RGBA_16ui,                    core::u32{ GL_RGBA16UI }                    },
	{ types::tex_format::RGBA_16_snorm,                core::u32{ GL_RGBA16_SNORM }                },
	{ types::tex_format::RGBA_8,                       core::u32{ GL_RGBA8 }                       },
	{ types::tex_format::RGBA_8i,                      core::u32{ GL_RGBA8I }                      },
	{ types::tex_format::RGBA_8ui,                     core::u32{ GL_RGBA8UI }                     },
	{ types::tex_format::RGBA_8_snorm,                 core::u32{ GL_RGBA8_SNORM }                 },
	{ types::tex_format::SRGB_8_alpha8,                core::u32{ GL_SRGB8_ALPHA8 }                },
	{ types::tex_format::SRGB_8,                       core::u32{ GL_SRGB8 }                       },
	{ types::tex_format::RGB_32f,                      core::u32{ GL_RGB32F }                      },
	{ types::tex_format::RGB_32i,                      core::u32{ GL_RGB32I }                      },
	{ types::tex_format::RGB_32ui,                     core::u32{ GL_RGB32UI }                     },
	{ types::tex_format::RGB_16,                       core::u32{ GL_RGB16 }                       },
	{ types::tex_format::RGB_16f,                      core::u32{ GL_RGB16F }                      },
	{ types::tex_format::RGB_16i,                      core::u32{ GL_RGB16I }                      },
	{ types::tex_format::RGB_16ui,                     core::u32{ GL_RGB16UI }                     },
	{ types::tex_format::RGB_16_snorm,                 core::u32{ GL_RGB16_SNORM }                 },
	{ types::tex_format::RGB_8,                        core::u32{ GL_RGB8 }                        },
	{ types::tex_format::RGB_8i,                       core::u32{ GL_RGB8I }                       },
	{ types::tex_format::RGB_8ui,                      core::u32{ GL_RGB8UI }                      },
	{ types::tex_format::RGB_8_snorm,                  core::u32{ GL_RGB8_SNORM }                  },
	{ types::tex_format::RGB_10_a2,                    core::u32{ GL_RGB10_A2 }                    },
	{ types::tex_format::RGB_10_a2ui,                  core::u32{ GL_RGB10_A2UI }                  },
	{ types::tex_format::RGB_9_e5,                     core::u32{ GL_RGB9_E5 }                     },
	{ types::tex_format::RG_32f,                       core::u32{ GL_RG32F }                       },
	{ types::tex_format::RG_32i,                       core::u32{ GL_RG32I }                       },
	{ types::tex_format::RG_32ui,                      core::u32{ GL_RG32UI }                      },
	{ types::tex_format::RG_16,                        core::u32{ GL_RG16 }                        },
	{ types::tex_format::RG_16f,                       core::u32{ GL_RG16F }                       },
	{ types::tex_format::RG_8,                         core::u32{ GL_RG8 }                         },
	{ types::tex_format::RG_8i,                        core::u32{ GL_RG8I }                        },
	{ types::tex_format::RG_8ui,                       core::u32{ GL_RG8UI }                       },
	{ types::tex_format::RG_16_snorm,                  core::u32{ GL_RG16_SNORM }                  },
	{ types::tex_format::RG_8_snorm,                   core::u32{ GL_RG8_SNORM }                   },
	{ types::tex_format::R11f_G11f_B10f,               core::u32{ GL_R11F_G11F_B10F }              },
	{ types::tex_format::R_32f,                        core::u32{ GL_R32F }                        },
	{ types::tex_format::R_32i,                        core::u32{ GL_R32I }                        },
	{ types::tex_format::R_32ui,                       core::u32{ GL_R32UI }                       },
	{ types::tex_format::R_16f,                        core::u32{ GL_R16F }                        },
	{ types::tex_format::R_16i,                        core::u32{ GL_R16I }                        },
	{ types::tex_format::R_16ui,                       core::u32{ GL_R16UI }                       },
	{ types::tex_format::R_8,                          core::u32{ GL_R8 }                          },
	{ types::tex_format::R_8i,                         core::u32{ GL_R8I }                         },
	{ types::tex_format::R_8ui,                        core::u32{ GL_R8UI }                        },
	{ types::tex_format::R_16_snorm,                   core::u32{ GL_R16_SNORM }                   },
	{ types::tex_format::R_8_snorm,                    core::u32{ GL_R8_SNORM }                    },
	{ types::tex_format::compressed_RG_RGTC2,          core::u32{ GL_COMPRESSED_RG_RGTC2 }         },
	{ types::tex_format::compressed_signed_RG_RGTC2,   core::u32{ GL_COMPRESSED_SIGNED_RG_RGTC2 }  },
	{ types::tex_format::compressed_red_RGTC1,         core::u32{ GL_COMPRESSED_RED_RGTC1 }        },
	{ types::tex_format::compressed_signed_red_RGTC1,  core::u32{ GL_COMPRESSED_SIGNED_RED_RGTC1 } },
	{ types::tex_format::depth_component_32f,          core::u32{ GL_DEPTH_COMPONENT32F }          },
	{ types::tex_format::depth_component_24,           core::u32{ GL_DEPTH_COMPONENT24 }           },
	{ types::tex_format::depth_component_16,           core::u32{ GL_DEPTH_COMPONENT16 }           },
	{ types::tex_format::depth_32f_stencil_8,          core::u32{ GL_DEPTH32F_STENCIL8 }           },
	{ types::tex_format::depth_24_stencil_8,           core::u32{ GL_DEPTH24_STENCIL8 }            },
};

const core::umap<types::tex_data_format, core::u32> gl_impl::gl_tex_data_format_map{
	{ types::tex_data_format::RED,                     core::u32{ GL_RED }                         },
	{ types::tex_data_format::RG,                      core::u32{ GL_RG }                          },
	{ types::tex_data_format::RGB,                     core::u32{ GL_RGB }                         },
	{ types::tex_data_format::BGR,                     core::u32{ GL_BGR }                         },
	{ types::tex_data_format::RGBA,                    core::u32{ GL_RGBA }                        },
	{ types::tex_data_format::BGRA,                    core::u32{ GL_BGRA }                        },
};


const core::umap<mods::capabilities, core::u32> gl_impl::gl_capability_map{
	{ mods::capabilities::blend,                          core::u32{ GL_BLEND }                           },
	{ mods::capabilities::color_logic_op,                 core::u32{ GL_COLOR_LOGIC_OP }                  },
	{ mods::capabilities::cull_face,                      core::u32{ GL_CULL_FACE }                       },
	{ mods::capabilities::debug_output,                   core::u32{ GL_DEBUG_OUTPUT }                    },
	{ mods::capabilities::debug_output_synchronous,       core::u32{ GL_DEBUG_OUTPUT_SYNCHRONOUS }        },
	{ mods::capabilities::depth_clamp,                    core::u32{ GL_DEPTH_CLAMP }                     },
	{ mods::capabilities::depth_test,                     core::u32{ GL_DEPTH_TEST }                      },
	{ mods::capabilities::dither,                         core::u32{ GL_DITHER }                          },
	{ mods::capabilities::framebuffer_srgb,               core::u32{ GL_FRAMEBUFFER_SRGB }                },
	{ mods::capabilities::line_smooth,                    core::u32{ GL_LINE_SMOOTH }                     },
	{ mods::capabilities::multisample,                    core::u32{ GL_MULTISAMPLE }                     },
	{ mods::capabilities::polygon_offset_fill,            core::u32{ GL_POLYGON_OFFSET_FILL }             },
	{ mods::capabilities::polygon_offset_line,            core::u32{ GL_POLYGON_OFFSET_LINE }             },
	{ mods::capabilities::polygon_offset_point,           core::u32{ GL_POLYGON_OFFSET_POINT }            },
	{ mods::capabilities::polygon_smooth,                 core::u32{ GL_POLYGON_SMOOTH }                  },
	{ mods::capabilities::primitive_restart,              core::u32{ GL_PRIMITIVE_RESTART }               },
	{ mods::capabilities::primitive_restart_fixed_index,  core::u32{ GL_PRIMITIVE_RESTART_FIXED_INDEX }   },
	{ mods::capabilities::rasterizer_discard,             core::u32{ GL_RASTERIZER_DISCARD }              },
	{ mods::capabilities::sample_alpha_to_coverage,       core::u32{ GL_SAMPLE_ALPHA_TO_COVERAGE }        },
	{ mods::capabilities::sample_alpha_to_one,            core::u32{ GL_SAMPLE_ALPHA_TO_ONE }             },
	{ mods::capabilities::sample_coverage,                core::u32{ GL_SAMPLE_COVERAGE }                 },
	{ mods::capabilities::sample_mask,                    core::u32{ GL_SAMPLE_MASK }                     },
	{ mods::capabilities::scissor_test,                   core::u32{ GL_SCISSOR_TEST }                    },
	{ mods::capabilities::stencil_test,                   core::u32{ GL_STENCIL_TEST }                    },
	{ mods::capabilities::texture_cube_map_seamless,      core::u32{ GL_TEXTURE_CUBE_MAP_SEAMLESS }       },
	{ mods::capabilities::program_point_size,             core::u32{ GL_PROGRAM_POINT_SIZE }              },

#if defined(GL_CLIP_DISTANCE)
	{ mods::capabilities::clip_distance,                  core::u32{ GL_CLIP_DISTANCE }                   },
#endif
#if defined(GL_CLIP_DISTANCE)
	{ mods::capabilities::sample_shading,                 core::u32{ GL_SAMPLE_SHADING }                  },
#endif
};

const core::umap<mods::blend::function, core::u32> gl_impl::gl_blend_function_map{
	{ mods::blend::function::zero,                        core::u32{ GL_ZERO }                     },
	{ mods::blend::function::one,                         core::u32{ GL_ONE }                      },
	{ mods::blend::function::src_color,                   core::u32{ GL_SRC_COLOR }                },
	{ mods::blend::function::one_minus_src_color,         core::u32{ GL_ONE_MINUS_SRC_COLOR }      },
	{ mods::blend::function::dst_color,                   core::u32{ GL_DST_COLOR }                },
	{ mods::blend::function::one_minus_dst_color,         core::u32{ GL_ONE_MINUS_DST_COLOR }      },
	{ mods::blend::function::src_alpha,                   core::u32{ GL_SRC_ALPHA }                },
	{ mods::blend::function::one_minus_src_alpha,         core::u32{ GL_ONE_MINUS_SRC_ALPHA }      },
	{ mods::blend::function::dst_alpha,                   core::u32{ GL_DST_ALPHA }                },
	{ mods::blend::function::one_minus_dst_alpha,         core::u32{ GL_ONE_MINUS_DST_ALPHA }      },
	{ mods::blend::function::constant_color,              core::u32{ GL_CONSTANT_COLOR }           },
	{ mods::blend::function::one_minus_constant_color,    core::u32{ GL_ONE_MINUS_CONSTANT_COLOR } },
	{ mods::blend::function::constant_alpha,              core::u32{ GL_CONSTANT_ALPHA }           },
	{ mods::blend::function::one_minus_constant_alpha,    core::u32{ GL_ONE_MINUS_CONSTANT_ALPHA } },
};

const core::umap<mods::blend::equation, core::u32> gl_impl::gl_blend_equation_map{
	{ mods::blend::equation::add,                         core::u32{ GL_FUNC_ADD }                 },
	{ mods::blend::equation::subtract,                    core::u32{ GL_FUNC_SUBTRACT }            },
	{ mods::blend::equation::reverse_subtract,            core::u32{ GL_FUNC_REVERSE_SUBTRACT }    },
	{ mods::blend::equation::min,                         core::u32{ GL_MIN }                      },
	{ mods::blend::equation::max,                         core::u32{ GL_MAX }                      },
};

const core::umap<mods::depth::function, core::u32> gl_impl::gl_depth_function_map{
	{ mods::depth::function::never,                       core::u32{ GL_NEVER }                    },
	{ mods::depth::function::less,                        core::u32{ GL_LESS }                     },
	{ mods::depth::function::equal,                       core::u32{ GL_EQUAL }                    },
	{ mods::depth::function::lequal,                      core::u32{ GL_LEQUAL }                   },
	{ mods::depth::function::greater,                     core::u32{ GL_GREATER }                  },
	{ mods::depth::function::not_equal,                   core::u32{ GL_NOTEQUAL }                 },
	{ mods::depth::function::gequal,                      core::u32{ GL_GEQUAL }                   },
};

template<class ResultValue, class MapKey, class MapValue>
constexpr ResultValue gl_value(const MapKey key, const core::umap<MapKey, MapValue> &map) {
	if (const auto found{ map.find(key) }; found != map.end()) {
		return static_cast<ResultValue>(found->second);
	}
	return ResultValue{ GL_NONE };
}


void GLAPIENTRY debug_msg_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
		[[maybe_unused]] GLsizei length, const GLchar *message, const void*) {

	if (type != GL_DEBUG_TYPE_ERROR) return;

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

	spdlog::error("[GL] [{}] [{}] ID: {}; message: {}", str_severity, str_source, id, msg);
}

bool gl_impl::initialize(controller::get_process_address_function function) {
	spdlog::info("[{}] Initializing OpenGL", class_name);

	if (!gladLoadGL((GLADloadfunc)function)) {
		spdlog::critical("[{}] Failed to initialize GLAD.", class_name);
		destroy();
		return false;
	}

	if (spdlog::get_level() == spdlog::level::debug) {
		// During init, enable debug output
		enable(mods::capabilities::debug_output);
		glDebugMessageCallback(debug_msg_callback, nullptr);
	}

	spdlog::info("[{}] Initialization has finished", class_name);

	GLint value{};
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value);
	max_texture_units = static_cast<core::u32>(value);

	return true;
}

void gl_impl::destroy() { }

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
	[[maybe_unused]] core::u32 EBO;

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
	} else if (info == typeid(glm::vec2)) {
		auto vec{ std::any_cast<glm::vec2>(value) };
		glUniform2fv(location, 1, glm::value_ptr(vec));
	} else if (info == typeid(glm::vec3)) {
		auto vec{ std::any_cast<glm::vec3>(value) };
		glUniform3fv(location, 1, glm::value_ptr(vec));
	} else if (info == typeid(glm::vec4)) {
		auto vec{ std::any_cast<glm::vec4>(value) };
		glUniform4fv(location, 1, glm::value_ptr(vec));
	} else if (info == typeid(glm::mat4)) {
		auto mat{ std::any_cast<glm::mat4>(value) };
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	} else if (info == typeid(glm::mat3)) {
		auto mat{ std::any_cast<glm::mat3>(value) };
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	} else {
		spdlog::error("[{}] Invalid uniform type: {}", class_name, info.name());
	}
}

void gl_impl::set_texture_image_ext(types::object::ref texture,
		const types::tex_target target, const core::i32 size,
		const types::tex_format internal_format, const types::tex_data_format data_format,
		const core::u8 *data) {
	if (data == nullptr || size <= 0) {
		spdlog::error("[{}] Texture data is null or width/height is less than zero", class_name);
		return;
	}
	if (!setup_texture(texture, target, internal_format, data_format)) {
		spdlog::error("[{}] Cannot setup texture", class_name);
		return;
	}

	const auto gl_target{ gl_value<GLenum>(target, gl_tex_target_map) };
	const auto gl_internal_format{ gl_value<GLenum>(internal_format, gl_tex_format_map) };
	const auto gl_data_format{ gl_value<GLenum>(data_format, gl_tex_data_format_map) };

	texture->set_property("width", size);

	bind_texture(texture, 0);
	glTexImage1D(gl_target, 0, gl_internal_format, size, 0, gl_data_format, GL_UNSIGNED_BYTE, data);
	unbind_texture(texture);
}

void gl_impl::set_texture_image_ext(types::object::ref texture,
		const types::tex_target target, const glm::i32vec2 &size,
		const types::tex_format internal_format, const types::tex_data_format data_format,
		const core::u8 *data) {
	if (data == nullptr || size.x <= 0 || size.y <= 0) {
		spdlog::error("[{}] Texture data is null or width/height is less than zero", class_name);
		return;
	}
	if (!setup_texture(texture, target, internal_format, data_format)) {
		spdlog::error("[{}] Cannot setup texture", class_name);
		return;
	}

	const auto gl_target{ gl_value<GLenum>(target, gl_tex_target_map) };
	const auto gl_internal_format{ gl_value<GLenum>(internal_format, gl_tex_format_map) };
	const auto gl_data_format{ gl_value<GLenum>(data_format, gl_tex_data_format_map) };

	texture->set_property("width", size.x);
	texture->set_property("height", size.y);

	bind_texture(texture, 0);
	glTexImage2D(gl_target, 0, gl_internal_format, size.x, size.y, 0, gl_data_format, GL_UNSIGNED_BYTE, data);
	unbind_texture(texture);
}

void gl_impl::set_texture_image_ext(types::object::ref texture,
		const types::tex_target target, const glm::i32vec3 &size,
		const types::tex_format internal_format, const types::tex_data_format data_format,
		const core::u8 *data) {
	if (data == nullptr || size.x <= 0 || size.y <= 0 || size.z <= 0) {
		spdlog::error("[{}] Texture data is null or width/height is less than zero", class_name);
		return;
	}
	if (!setup_texture(texture, target, internal_format, data_format)) {
		spdlog::error("[{}] Cannot setup texture", class_name);
		return;
	}

	const auto gl_target{ gl_value<GLenum>(target, gl_tex_target_map) };
	const auto gl_internal_format{ gl_value<GLenum>(internal_format, gl_tex_format_map) };
	const auto gl_data_format{ gl_value<GLenum>(data_format, gl_tex_data_format_map) };

	texture->set_property("width", size.x);
	texture->set_property("height", size.y);
	texture->set_property("depth", size.z);

	bind_texture(texture, 0);
	glTexImage3D(gl_target, 0, gl_internal_format, size.x, size.y, size.z, 0, gl_data_format, GL_UNSIGNED_BYTE, data);
	unbind_texture(texture);
}

void gl_impl::set_texture_image(types::object::ref tex, const core::types::image::ref &img,
		const types::tex_target target, const types::tex_format data_format) {
	if (img == nullptr) {
		spdlog::error("[{}] Image is null", class_name);
		return;
	}

	set_texture_image_ext(tex, target, glm::i32vec2{ img->width(), img->height() },
		data_format, types::texture::get_pixel_data_format(img), img->raw().data());
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

	const auto target{ texture->get_property<types::tex_type>(types::texture::param_type)
		.value_or(types::tex_type::texture_2d) };

	glBindTexture(gl_value<GLenum>(target, gl_tex_type_map), id);

	return true;
}

bool gl_impl::unbind_texture(const types::object::ref &texture) {
	if (texture == nullptr || !texture->valid()) {
		return false;
	}

	const auto target{ texture->get_property<types::tex_type>(types::texture::param_type)
		.value_or(types::tex_type::texture_2d) };
	glBindTexture(gl_value<GLenum>(target, gl_tex_type_map), GLuint{ 0 });

	return true;
}

void gl_impl::set_raw_texture_parameter(const types::object::ref &texture,
	const std::type_info &param_type, const std::any param_value) {
	if (texture == nullptr || !texture->valid()) {
		return;
	}

	using namespace types;

	bind_texture(texture, 0);

	const auto target{ gl_value<GLenum>(texture->get_property<types::tex_type>(types::texture::param_type)
		.value_or(types::tex_type::texture_2d), gl_tex_type_map) };

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

	glBindVertexArray(VAO.value());

	if (const auto indices_count{ mesh->get_property<core::u32>("indices_count") }; indices_count.has_value()) {
		glDrawElements(GL_TRIANGLES, indices_count.value(), GL_UNSIGNED_INT, 0);
	} else {
		const auto vertices_count{ mesh->get_property<core::u32>("vertices_count") };
		glDrawArrays(GL_TRIANGLES, 0, vertices_count.value());
	}

	glBindVertexArray(0);
}

void gl_impl::viewport(const core::u32 x, const core::u32 y, const core::u32 width, const core::u32 height) noexcept {
	glViewport(x, y, width, height);
}

void gl_impl::enable(const mods::capabilities capability) {
	if (const auto found{ gl_value<GLenum>(capability, gl_capability_map) }; found != GL_NONE) {
		glEnable(found);
	} else {
		spdlog::error("[{}] Unhandled capability: {:x}", class_name, static_cast<core::u32>(capability));
	}
}
void gl_impl::enable(const mods::capabilities capability, const core::u32 value) {
	if (const auto found{ gl_value<GLenum>(capability, gl_capability_map) }; found != GL_NONE) {
		glEnablei(found, value);
	} else {
		spdlog::error("[{}] Unhandled capability: {:x}", class_name, static_cast<core::u32>(capability));
	}
}
void gl_impl::disable(const mods::capabilities capability) {
	if (const auto found{ gl_value<GLenum>(capability, gl_capability_map) }; found != GL_NONE) {
		glDisable(found);
	} else {
		spdlog::error("[{}] Unhandled capability: {:x}", class_name, static_cast<core::u32>(capability));
	}
}
void gl_impl::disable(const mods::capabilities capability, const core::u32 value) {
	if (const auto found{ gl_value<GLenum>(capability, gl_capability_map) }; found != GL_NONE) {
		glDisablei(found, value);
	} else {
		spdlog::error("[{}] Unhandled capability: {:x}", class_name, static_cast<core::u32>(capability));
	}
}
bool gl_impl::is_enabled(const mods::capabilities capability) const {
	if (const auto found{ gl_value<GLenum>(capability, gl_capability_map) }; found != GL_NONE) {
		return glIsEnabled(found) == GL_TRUE;
	}
	spdlog::error("[{}] Unhandled capability: {:x}", class_name, static_cast<core::u32>(capability));
	return false;
}
bool gl_impl::is_enabled(const mods::capabilities capability, const core::u32 value) const {
	if (const auto found{ gl_value<GLenum>(capability, gl_capability_map) }; found != GL_NONE) {
		return glIsEnabledi(found, value) == GL_TRUE;
	}
	spdlog::error("[{}] Unhandled capability: {:x}", class_name, static_cast<core::u32>(capability));
	return false;
}
core::i32 gl_impl::capability_value(const mods::capabilities capability) const {
	if (const auto found{ gl_value<GLenum>(capability, gl_capability_map) }; found != GL_NONE) {
		core::i32 capability_value_out{};
		glGetIntegerv(found, &capability_value_out);
		return capability_value_out;
	}
	spdlog::error("[{}] Unhandled capability: {:x}", class_name, static_cast<core::u32>(capability));
	return false;
}

void gl_impl::set_blend_color(const glm::vec4 &color) {
	glBlendColor(color.r, color.g, color.b, color.a);
}

glm::vec4 gl_impl::get_blend_color() {
	glm::vec4 color{};
	glGetFloatv(GL_BLEND_COLOR, glm::value_ptr(color));
	return color;
}

void gl_impl::set_blend_function(const mods::blend::function src, const mods::blend::function dest) {
	glBlendFunc(
		gl_value<GLenum>(src, gl_blend_function_map),
		gl_value<GLenum>(dest, gl_blend_function_map)
	);
}

std::pair<mods::blend::function, mods::blend::function> gl_impl::get_blend_function() {
	GLint src{}, dest{};
	glGetIntegerv(GL_BLEND_SRC_RGB, &src);
	glGetIntegerv(GL_BLEND_DST_RGB, &dest);

	std::optional<mods::blend::function> found_src{};
	std::optional<mods::blend::function> found_dest{};
	for (const auto [func, value] : gl_blend_function_map) {
		if (!found_src.has_value() && value == static_cast<GLuint>(src)) {
			found_src = func;
		}
		if (!found_dest.has_value() && value == static_cast<GLuint>(dest)) {
			found_dest = func;
		}
		if (found_src.has_value() && found_dest.has_value()) {
			return std::make_pair(found_src.value(), found_dest.value());
		}
	}
	spdlog::error("[{}] Unhandled blend function: src: {}, dest: {}", class_name, src, dest);
	return std::make_pair(mods::blend::function::one, mods::blend::function::zero);
}

void gl_impl::set_blend_equation(const mods::blend::equation equation) {
	glBlendEquation(gl_value<GLenum>(equation, gl_blend_equation_map));
}

mods::blend::equation gl_impl::get_blend_equation() {
	GLint equation{};
	glGetIntegerv(GL_BLEND_EQUATION_RGB, &equation);
	for (const auto [func, value] : gl_blend_equation_map) {
		if (value == static_cast<GLuint>(equation)) {
			return func;
		}
	}
	spdlog::error("[{}] Unhandled blend equation: {}", class_name, equation);
	return mods::blend::equation::add;
}

void gl_impl::set_blend_function_separate(
	const mods::blend::function src_rgb, const mods::blend::function dest_rgb,
	const mods::blend::function src_alpha, const mods::blend::function dest_alpha) {
	glBlendFuncSeparate(
		gl_value<GLenum>(src_rgb, gl_blend_function_map),
		gl_value<GLenum>(dest_rgb, gl_blend_function_map),
		gl_value<GLenum>(src_alpha, gl_blend_function_map),
		gl_value<GLenum>(dest_alpha, gl_blend_function_map)
	);
}

std::array<mods::blend::function, 4> gl_impl::get_blend_function_separate() {
	GLint src_rgb{}, dest_rgb{}, src_alpha{}, dest_alpha{};
	glGetIntegerv(GL_BLEND_SRC_RGB, &src_rgb);
	glGetIntegerv(GL_BLEND_DST_RGB, &dest_rgb);
	glGetIntegerv(GL_BLEND_SRC_ALPHA, &src_alpha);
	glGetIntegerv(GL_BLEND_DST_ALPHA, &dest_alpha);

	std::optional<mods::blend::function> found_src_rgb{};
	std::optional<mods::blend::function> found_dest_rgb{};
	std::optional<mods::blend::function> found_src_alpha{};
	std::optional<mods::blend::function> found_dest_alpha{};
	for (const auto [func, value] : gl_blend_function_map) {
		if (!found_src_rgb.has_value() && value == static_cast<GLuint>(src_rgb)) {
			found_src_rgb = func;
		}
		if (!found_dest_rgb.has_value() && value == static_cast<GLuint>(dest_rgb)) {
			found_dest_rgb = func;
		}
		if (!found_src_alpha.has_value() && value == static_cast<GLuint>(src_alpha)) {
			found_src_alpha = func;
		}
		if (!found_dest_alpha.has_value() && value == static_cast<GLuint>(dest_alpha)) {
			found_dest_alpha = func;
		}
		if (found_src_rgb.has_value() && found_dest_rgb.has_value() &&
			found_src_alpha.has_value() && found_dest_alpha.has_value()) {
			return std::array<mods::blend::function, 4>{
				found_src_rgb.value(), found_dest_rgb.value(),
				found_src_alpha.value(), found_dest_alpha.value()
			};
		}
	}
	spdlog::error(
		"[{}] Unhandled blend function: src_rgb: {}, dest_rgb: {}, src_alpha: {}, dest_alpha: {}",
		class_name, src_rgb, dest_rgb, src_alpha, dest_alpha
	);
	return std::array<mods::blend::function, 4>{
		mods::blend::function::one, mods::blend::function::zero,
		mods::blend::function::one, mods::blend::function::zero
	};
}

void gl_impl::set_blend_equation_separate(
		const mods::blend::equation equation_rgb, const mods::blend::equation equation_alpha) {
	glBlendEquationSeparate(
		gl_value<GLenum>(equation_rgb, gl_blend_equation_map),
		gl_value<GLenum>(equation_alpha, gl_blend_equation_map)
	);
}

std::pair<mods::blend::equation, mods::blend::equation> gl_impl::get_blend_equation_separate() {
	GLint equation_rgb{}, equation_alpha{};
	glGetIntegerv(GL_BLEND_EQUATION_RGB, &equation_rgb);
	glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &equation_alpha);

	std::optional<mods::blend::equation> found_equation_rgb{};
	std::optional<mods::blend::equation> found_equation_alpha{};
	for (const auto [func, value] : gl_blend_equation_map) {
		if (!found_equation_rgb.has_value() && value == static_cast<GLuint>(equation_rgb)) {
			found_equation_rgb = func;
		}
		if (!found_equation_alpha.has_value() && value == static_cast<GLuint>(equation_alpha)) {
			found_equation_alpha = func;
		}
		if (found_equation_rgb.has_value() && found_equation_alpha.has_value()) {
			return std::make_pair(found_equation_rgb.value(), found_equation_alpha.value());
		}
	}
	spdlog::error("[{}] Unhandled blend equation: rgb: {}, alpha: {}", class_name, equation_rgb, equation_alpha);
	return std::make_pair(mods::blend::equation::add, mods::blend::equation::add);
}


void gl_impl::set_depth_mask(const bool value) {
	glDepthMask(value ? GL_TRUE : GL_FALSE);
}
bool gl_impl::get_depth_mask() const {
	GLboolean value{};
	glGetBooleanv(GL_DEPTH_WRITEMASK, &value);
	return value == GL_TRUE;
}

void gl_impl::set_depth_function(const mods::depth::function func) {
	if (const auto value{ gl_value<GLenum>(func, gl_depth_function_map) }; value != GL_NONE) {
		glDepthFunc(value);
	} else {
		spdlog::error("[{}] Unhandled depth function: {:x}", class_name, static_cast<core::u32>(func));
	}
}
mods::depth::function gl_impl::get_depth_function() const {
	GLint value{};
	glGetIntegerv(GL_DEPTH_FUNC, &value);
	const auto found{ std::find_if(std::begin(gl_depth_function_map), std::end(gl_depth_function_map),
		[value](const auto &item) { return static_cast<GLint>(item.second) == value; }) };
	if (found != std::end(gl_depth_function_map)) {
		return found->first;
	}
	spdlog::error("[{}] Unhandled depth function: {:x}", class_name, value);
	return mods::depth::function::never;
}

void gl_impl::set_depth_range(const core::f16 min, const core::f16 max) {
	glDepthRangef(std::max(0.0f, min), std::min(1.0f, max));
}
std::pair<core::f16, core::f16> gl_impl::get_depth_range() const {
	core::f16 values[2]{};
	glGetFloati_v(GL_DEPTH_RANGE, 2, values);
	return std::make_pair(values[0], values[1]);
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

bool gl_impl::setup_texture(types::object::ref texture, const types::texture::target target,
		const types::tex_format internal_format, const types::tex_data_format data_format) const {

	if (texture == nullptr || !texture->valid()) {
		spdlog::error("[{}] Texture is null or invalid", class_name);
		return false;
	}

	const auto gl_target{ gl_value<GLenum>(target, gl_tex_target_map) };
	if (gl_target == GL_NONE) {
		/// @todo: Use magic_enum for target
		spdlog::error("[{}] Unhandled texture target: {}", class_name, static_cast<core::u32>(target));
		return false;
	}
	const auto gl_internal_format{ gl_value<GLenum>(internal_format, gl_tex_format_map) };
	if (gl_internal_format == GL_NONE) {
		/// @todo: Use magic_enum for internal_format
		spdlog::error("[{}] Unhandled texture format: {}", class_name, static_cast<core::u32>(internal_format));
		return false;
	}
	const auto gl_data_format{ gl_value<GLenum>(data_format, gl_tex_data_format_map) };
	if (gl_data_format == GL_NONE) {
		/// @todo: Use magic_enum for data_format
		spdlog::error("[{}] Unhandled texture data format: {}", class_name, static_cast<core::u32>(data_format));
		return false;
	}

	texture->set_property("gl_target", gl_target);
	texture->set_property("gl_internal_format", gl_internal_format);
	texture->set_property("gl_data_format", gl_data_format);
	return true;
}

core::i32 gl_impl::translate_texture_channel(const core::i32 channel) const noexcept {
	switch(channel) {
		case 1: return GL_RED;
		case 2: return GL_RG;
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
		default:
			spdlog::error("[{}] Unhandled channel: {}", class_name, channel);
			break;
	}
	return GL_RED;
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

	glTexParameteri(target, GL_DEPTH_STENCIL_TEXTURE_MODE, gl_value<GLenum>(*value_ptr, mode_map));
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

	glTexParameteri(target, GL_TEXTURE_COMPARE_FUNC, gl_value<GLenum>(*value_ptr, lookup));
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
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, gl_value<GLenum>(*value_ptr, lookup));
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
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, gl_value<GLenum>(*value_ptr, lookup));
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
	glTexParameterf(target, gl_value<GLenum>(level, level_map), static_cast<GLfloat>(*value_ptr));
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
	glTexParameteri(target, gl_value<GLenum>(type, mode_lookup), gl_value<GLenum>(*value_ptr, channel_lookup));
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
	glTexParameteri(target, gl_value<GLenum>(type, type_lookup), gl_value<GLenum>(*value_ptr, mode_lookup));
}

} // namespace golxzn::graphics
