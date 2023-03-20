#include <spdlog/spdlog.h>
#include <golxzn/core/resources/manager.hpp>

#include "golxzn/graphics/types/texture.hpp"

#include "golxzn/graphics/controller/controller.hpp"

namespace golxzn::graphics::types {

texture::ref texture::make(const type tex_type, const std::string &path) {
	return std::make_shared<texture>(tex_type, path);
}
texture::ref texture::make(const std::string &name, core::bytes &&data) {
	return std::make_shared<texture>(name, std::move(data));
}
texture::ref texture::make(const std::string &name, cubemap_array<core::bytes> &&data) {
	return std::make_shared<texture>(name, std::move(data));
}


texture::texture(const type tex_type, const std::string &path)
	: named{ core::fs::path{ path }.stem().string() } {

	if (!make_texture(tex_type)) return;

	if (tex_type == type::cube_map) {
		cubemap_array<std::string> faces;

		const auto ext_pos{ path.find_last_of('.') };
		if (ext_pos == std::string::npos) {
			spdlog::error("[{}] [{}] The path has not extension: {}", class_name, full_name(), path);
			return;
		}
		const auto ext{ path.substr(ext_pos) };
		const auto path_without_ext{ path.substr(0, ext_pos) };
		static constexpr std::string_view face_names[]{
			"right", "left", "bottom", "top", "front", "back"
		};

		for (core::u32 i{}; i < cube_map_faces; ++i) {
			faces[i] = path_without_ext + '/' + std::string{ face_names[i] } + ext;
		}
		mObject->set_property(param_path, std::move(faces));
	} else {
		mObject->set_property(param_path, path);
	}

	generate();
}

texture::texture(const std::string &name, core::bytes &&data)
	: named{ name } {

	if (!make_texture(type::texture_2d)) return;

	mObject->set_property(param_data, std::move(data));
	generate();
}

texture::texture(const std::string &name, cubemap_array<core::bytes> &&data)
	: named{ name } {

	if (!make_texture(type::cube_map)) return;

	mObject->set_property(param_data, std::move(data));
	generate();
}

void texture::bind(const core::u32 unit) const noexcept {
	if (!valid()) return;

	if (auto api{ controller::api() }; api) {
		api->bind_texture(mObject, unit);
	}
}
void texture::unbind() const noexcept {
	if (!valid()) return;

	if (auto api{ controller::api() }; api) {
		api->unbind_texture(mObject);
	}
}

texture::type texture::get_type() const noexcept {
	if (!valid()) return type::invalid;
	return mObject->get_property<type>(param_type).value_or(type::invalid);
}

bool texture::valid() const noexcept {
	return mObject != nullptr && mObject->valid();
}

object::id_t texture::id() const noexcept {
	if (!valid()) return 0;
	return mObject->id();
}

core::u32 texture::width() const noexcept {
	if (!valid()) return 0;
	return mObject->get_property<core::u32>(param_width).value_or(0);
}
core::u32 texture::height() const noexcept {
	if (!valid()) return 0;
	return mObject->get_property<core::u32>(param_height).value_or(0);
}

void texture::set_data(const core::bytes &data) {
	if (!valid()) return;
	if (get_type() != type::texture_2d) {
		mObject->set_property(param_type, type::texture_2d);
	}
	mObject->set_property(param_data, data);
}
void texture::set_data(core::bytes &&data) {
	if (!valid()) return;
	if (get_type() != type::texture_2d) {
		mObject->set_property(param_type, type::texture_2d);
	}
	mObject->set_property(param_data, std::move(data));
}
void texture::set_data(const cubemap_array<core::bytes> &data) {
	if (!valid()) return;
	if (get_type() != type::cube_map) {
		mObject->set_property(param_type, type::cube_map);
	}
	mObject->set_property(param_data, data);
}
void texture::set_data(cubemap_array<core::bytes> &&data) {
	if (!valid()) return;
	if (get_type() != type::cube_map) {
		mObject->set_property(param_type, type::cube_map);
	}
	mObject->set_property(param_data, std::move(data));
}

bool texture::generate(const bool setup_default_params) {
	if (!valid()) return false;

	auto api{ controller::api() };
	if (api == nullptr) {
		spdlog::error("[{}] [{}] Cannot generate! No API instance", class_name, full_name());
		return false;
	}

	mObject->set_property("setup_default_params", setup_default_params);

	switch(get_type()) {
		case type::texture_2d: {
			const auto optional_path{ mObject->get_property<std::string>(param_path) };
			if (!optional_path.has_value()) {
				spdlog::error("[{}] [{}] The texture has no path", class_name, full_name());
				return false;
			}

			const auto data{ core::res_man::load_binary(optional_path.value()) };
			return api->make_texture_image_2d(mObject, data);
		} break;

		case type::cube_map: {
			const auto optional_paths{ mObject->get_property<cubemap_array<std::string>>(param_path) };
			if (!optional_paths.has_value()) {
				spdlog::error("[{}] [{}] The texture has no path", class_name, full_name());
				return false;
			}
			cubemap_array<core::bytes> data;
			const auto paths{ optional_paths.value() };
			std::transform(std::begin(paths), std::end(paths), std::begin(data), [this] (const auto &path) {
				spdlog::debug("[{}] [{}] Loading cubemap face: {}", class_name, full_name(), path);
				return core::res_man::load_binary(path);
			} );

			static const auto is_empty{ [] (const auto &d) { return d.empty(); } };
			if (std::any_of(std::begin(data), std::end(data), is_empty)) {
				spdlog::error("[{}] [{}] One or more cube_map faces could not be loaded", class_name, full_name());
				return false;
			}
			return api->make_texture_image_2d(mObject, data);
		} break;
		default: break;
	}
	return false;
}

bool texture::generate_mip_maps() {
	if (!valid()) return false;
	if (auto api{ controller::api() }; api) {
		api->generate_mip_maps(mObject);
		return true;
	}
	return false;
}

bool texture::make_texture(const type tex_type) {
	if (auto api{ controller::api() }; api) {
		mObject = api->make_texture();
		mObject->set_property(param_type, tex_type);
		spdlog::debug("[{}] [{}] Created a new {} texture object", class_name, full_name(), mObject->id());
		return true;
	}

	spdlog::critical("[{}] [{}] controller::api is nullptr", class_name, full_name());
	return false;
}

//====================================== texture parameters ======================================//

texture::parameter::parameter(object::ref object_ref) noexcept
	: texture_object{ std::move(object_ref) } {}

object::ref texture::parameter::get_object() const noexcept {
	if (controller::api() == nullptr) {
		spdlog::critical("[{}] Cannot check texture parameter validity without an API instance", class_name);
		return nullptr;
	}
	if (auto texture{ texture_object.lock() }; texture != nullptr) {
		return texture;
	}
	spdlog::error("[{}] Cannot check texture parameter validity without a valid object", class_name);
	return nullptr;
}

texture::depth_stencil_texture_mode &texture::depth_stencil_texture_mode::operator=(const mode value) {
	if (auto texture{ get_object() }; texture != nullptr) {
		controller::api()->set_texture_parameter<depth_stencil_texture_mode>(texture, value);
	}
	return *this;
}
texture::base_level &texture::base_level::operator=(const core::u32 value) {
	if (auto texture{ get_object() }; texture != nullptr) {
		controller::api()->set_texture_parameter<base_level>(texture, value);
	}
	return *this;
}
texture::compare_function &texture::compare_function::operator=(const function value) {
	if (auto texture{ get_object() }; texture != nullptr) {
		controller::api()->set_texture_parameter<compare_function>(texture, value);
	}
	return *this;
}
texture::compare_mode &texture::compare_mode::operator=(const mode value) {
	if (auto texture{ get_object() }; texture != nullptr) {
		controller::api()->set_texture_parameter<compare_mode>(texture, value);
	}
	return *this;
}
texture::min_filter &texture::min_filter::operator=(const filter value) {
	if (auto texture{ get_object() }; texture != nullptr) {
		controller::api()->set_texture_parameter<min_filter>(texture, value);
	}
	return *this;
}
texture::mag_filter &texture::mag_filter::operator=(const filter value) {
	if (auto texture{ get_object() }; texture != nullptr) {
		controller::api()->set_texture_parameter<mag_filter>(texture, value);
	}
	return *this;
}
texture::max_level &texture::max_level::operator=(const core::u32 value) {
	if (auto texture{ get_object() }; texture != nullptr) {
		controller::api()->set_texture_parameter<max_level>(texture, value);
	}
	return *this;
}

texture::lod &texture::lod::operator[](const level new_mode) noexcept {
	current_mode = new_mode;
	return *this;
}
texture::lod &texture::lod::operator=(const core::f32 value) {
	if (auto texture{ get_object() }; texture != nullptr) {
		auto api{ controller::api() };
		switch(current_mode) {
			case level::min: api->set_texture_parameter<min_level>(texture, value); break;
			case level::max: api->set_texture_parameter<max_level>(texture, value); break;
			case level::bias: api->set_texture_parameter<bias>(texture, value); break;
			default: break;
		}
	}
	return *this;
}

texture::swizzle &texture::swizzle::operator[](const type new_type) noexcept {
	current_type = new_type;
	return *this;
}
texture::swizzle &texture::swizzle::operator=(const channel value) {
	if (auto texture{ get_object() }; texture != nullptr) {
		auto api{ controller::api() };
		switch(current_type) {
			case type::red: api->set_texture_parameter<red_type>(texture, value); break;
			case type::green: api->set_texture_parameter<green_type>(texture, value); break;
			case type::blue: api->set_texture_parameter<blue_type>(texture, value); break;
			case type::alpha: api->set_texture_parameter<alpha_type>(texture, value); break;
			default: break;
		}
	}
	return *this;
}

texture::wrap &texture::wrap::operator[](const type new_type) noexcept {
	current_type = new_type;
	return *this;
}

texture::wrap &texture::wrap::operator=(const mode value) {
	if (auto texture{ get_object() }; texture != nullptr) {
		auto api{ controller::api() };
		switch(current_type) {
			case type::s: api->set_texture_parameter<s_type>(texture, value); break;
			case type::t: api->set_texture_parameter<t_type>(texture, value); break;
			case type::r: api->set_texture_parameter<r_type>(texture, value); break;
			default: break;
		}
	}
	return *this;
}

} // namespace golxzn::graphics::types
