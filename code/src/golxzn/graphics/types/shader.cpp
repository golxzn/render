#include <spdlog/spdlog.h>
#include <golxzn/core/resources/manager.hpp>
#include <golxzn/core/utils/traits.hpp>

#include "golxzn/graphics/types/shader.hpp"

#include "golxzn/graphics/controller/controller.hpp"

namespace golxzn::graphics::types {

shader::ref shader::make(const std::string_view path) {
	return std::make_shared<shader>(path);
}
shader::ref shader::make(std::string &&code, const type shader_type) {
	return std::make_shared<shader>(std::move(code), shader_type);
}

shader::shader(const shader &other)
	: mType{ other.mType }, mStatus{ other.mStatus }, mCode{ other.mCode }, mObject{ other.mObject } {
}

shader::shader(shader &&other) noexcept
	: mType{ other.mType }
	, mStatus{ other.mStatus }
	, mCode{ std::move(other.mCode) }
	, mObject{ std::move(other.mObject) } {
	other.clear();
}

shader::shader(const std::string_view path)
	: shader{ core::res_man::load_string(path), get_type_by_extension(path) } {
}

shader::shader(std::string &&code, const type shader_type)
	: mType{ shader_type }
	, mCode{ std::move(code) } {
	compile();
}

shader::~shader() {
	clear();
}

shader &shader::operator=(const shader &other) {
	if (this == &other) {
		return *this;
	}
	clear();
	mType = other.mType;
	mStatus = other.mStatus;
	mCode = other.mCode;
	mObject = other.mObject;
	return *this;
}

shader &shader::operator=(shader &&other) noexcept {
	if (this == &other) {
		return *this;
	}
	clear();
	mType = other.mType;
	mStatus = other.mStatus;
	mCode = std::move(other.mCode);
	mObject = std::move(other.mObject);
	other.clear();
	return *this;
}

shader::status shader::compile() {
	if (auto api{ controller::api() }; api) {
		mObject = api->make_shader(mType, mCode);
		mStatus = valid() ? status::compile_success : status::compile_failure;
		return mStatus;
	}

	spdlog::critical("[{}] controller::api is nullptr", class_name);
	mStatus = status::invalid;
	return mStatus;
}

void shader::clear() noexcept {
	mStatus = status::invalid;
	mType = type::invalid;
	mCode.clear();
	mObject.reset();
}

core::u32 shader::id() const noexcept {
	if (valid()) {
		return mObject->id();
	}
	return ctrl::object::invalid_id;
}
shader::type shader::get_type() const noexcept {
	return mType;
}
shader::status shader::get_status() const noexcept {
	return mStatus;
}
const std::string &shader::get_code() const noexcept {
	return mCode;
}
ctrl::object::ref shader::to_object() const noexcept {
	return mObject;
}

shader::status shader::set_code(std::string &&code) {
	if (code.empty()) {
		spdlog::warn("[{}] set_code: Shader code is empty", class_name);
		clear();
		return mStatus;
	}
	mCode = std::move(code);
	mStatus = status::need_to_compile;
	return mStatus;
}

shader::status shader::set_code(std::string &&code, const type shader_type) {
	mType = shader_type;
	if (core::traits::any_from(mType, type::count, type::invalid)) {
		// maybe it's a new shader
		spdlog::warn("[{}] set_code: {}", class_name,
			mType == type::count ? "AW DO NOT USE type::count AS A TYPE!" : "Invalid type");
		clear();
		return mStatus;
	}
	return set_code(std::move(code));
}

bool shader::valid() const noexcept {
	return mObject && mObject->valid();
}

shader::type shader::get_type_by_extension(const std::string_view filename) noexcept {
	if (filename.empty()) {
		spdlog::warn("[{}] File with no extension: '{}'", class_name, filename);
		return type::invalid;
	}

	static constexpr auto npos{ std::wstring_view::npos };
	static constexpr std::initializer_list types {
		type::vertex,
		type::tesselation_control,
		type::tesselation_evaluation,
		type::geometry,
		type::fragment,
		type::compute
	};

	for (const auto &type : types) {
		if (filename.rfind(get_extension(type)) != npos) {
			return type;
		}
	}

	return type::invalid;
}

std::string_view shader::get_extension(const type type) noexcept {
	switch (type) {
		case type::vertex:                 return ".vs";
		case type::tesselation_control:    return ".tcs";
		case type::tesselation_evaluation: return ".tes";
		case type::geometry:               return ".gs";
		case type::fragment:               return ".fs";
		case type::compute:                return ".cs";
		case type::count:
			spdlog::warn("[{}] get_extension: DO NOT USE type::count!", class_name);
			break;
		default: break;
	}
	return "";
}

std::string_view shader::type_to_str(const type type) noexcept {
	switch (type) {
		case type::vertex:                 return "vertex";
		case type::tesselation_control:    return "tesselation_control";
		case type::tesselation_evaluation: return "tesselation_evaluation";
		case type::geometry:               return "geometry";
		case type::fragment:               return "fragment";
		case type::compute:                return "compute";
		case type::count:
			spdlog::warn("[{}] type_to_str: DO NOT USE type::count!", class_name);
			break;

		default: break;
	}
	return "invalid";
}

} // namespace golxzn::graphics::types
