#include <spdlog/spdlog.h>

#include "golxzn/graphics/types/shader_program.hpp"
#include "golxzn/graphics/controller/controller.hpp"

namespace golxzn::graphics::types {

shader_program::ref shader_program::make() {
	return std::make_shared<shader_program>();
}
shader_program::ref shader_program::make(std::initializer_list<shader> &&shaders) {
	return std::make_shared<shader_program>(std::move(shaders));
}
shader_program::ref shader_program::make(std::initializer_list<shader::ref> &&shaders) {
	return std::make_shared<shader_program>(std::move(shaders));
}


shader_program::shader_program() {
	if (auto api{ controller::api() }; api) {
		mObject = api->make_program();
		mStatus = valid() ? status::need_to_link : status::invalid;
		return;
	}

	spdlog::critical("[{}] controller::api is nullptr", class_name);
	mStatus = status::invalid;
}

shader_program::shader_program(std::initializer_list<shader> &&shaders) : shader_program{} {
	if (!valid()) {
		return;
	}

	mAttachedShaders.reserve(shaders.size());
	for (auto &&shader : shaders) {
		if (const auto status{ attach(std::move(shader)) }; status != status::attach_success) {
			return;
		}
	}
}

shader_program::shader_program(std::initializer_list<shader::ref> &&shaders) : shader_program{} {
	if (!valid()) {
		return;
	}

	mAttachedShaders.reserve(shaders.size());
	for (auto &&shader : shaders) {
		if (const auto status{ attach(std::move(shader)) }; status != status::attach_success) {
			return;
		}
	}
}

void shader_program::clear() noexcept {
	mStatus = status::invalid;
	mObject.reset();
	mAttachedShaders.clear();
}

bool shader_program::is_attached(const shader &shader_obj) const noexcept {
	const auto shader_obj_id{ shader_obj.id() };
	return std::any_of(std::cbegin(mAttachedShaders), std::cend(mAttachedShaders),
		[&shader_obj_id](const shader::ref &ref) { return ref->id() == shader_obj_id; });
}
bool shader_program::is_attached(const shader::ref &shader_obj) const noexcept {
	if (shader_obj == nullptr) {
		return false;
	}
	return is_attached(*shader_obj);
}

shader_program::status shader_program::attach(shader &&shader_obj) noexcept {
	return attach(std::make_shared<shader>(std::move(shader_obj)));
}

shader_program::status shader_program::attach(const shader &shader_obj) noexcept {
	return attach(std::make_shared<shader>(shader_obj));
}

shader_program::status shader_program::attach(shader::ref shader_ref) noexcept {
	if (!valid()) {
		spdlog::warn("[{}] shader program is not valid", class_name);
		mStatus = status::invalid;
		return mStatus;
	}
	if (shader_ref != nullptr && !shader_ref->valid()) {
		spdlog::warn("[{}] Cannot attach invalid shader", class_name);
		return shader_program::status::attach_failure;
	}

	if (is_attached(shader_ref)) {
		spdlog::warn("[{}] shader is already attached to the shader program", class_name);
		return shader_program::status::attach_already;
	}

	if (auto api{ controller::api() }; api) {
		if (api->attach_shader(mObject, shader_ref->to_object())) {
			mAttachedShaders.emplace_back(std::move(shader_ref));
			mStatus = status::need_to_link;
			return status::attach_success;
		}
	}

	clear();
	return status::fatal_error;
}

shader_program::status shader_program::detach(const shader &shader_obj) noexcept {
	if (!valid()) {
		spdlog::warn("[{}] shader program is not valid", class_name);
		mStatus = status::invalid;
		return mStatus;
	}

	const auto shader_object{ shader_obj.to_object() };
	if (!erase_shader(shader_object)) {
		return shader_program::status::detach_failure;
	}

	if (auto api{ controller::api() }; api) {
		if (api->detach_shader(mObject, shader_object)) {
			mStatus = status::need_to_link;
			return status::detach_success;
		}
		return status::detach_failure;
	}

	clear();
	return status::fatal_error;
}

shader_program::status shader_program::detach(shader::ref shader_ref) noexcept {
	if (shader_ref == nullptr) {
		spdlog::warn("[{}] shader_ref is nullptr", class_name);
		return status::detach_failure;
	}
	return detach(*shader_ref);
}

shader_program::status shader_program::link() noexcept {
	if (!valid()) {
		spdlog::warn("[{}] shader program is not valid", class_name);
		mStatus = status::invalid;
		return mStatus;
	}

	if (mAttachedShaders.empty()) {
		spdlog::warn("[{}] shader program does not have attached shader", class_name);
		mStatus = status::link_failure;
		return mStatus;
	}

	if (auto api{ controller::api() }; api) {
		if (api->link_program(mObject)) {
			mStatus = status::link_success;
			return mStatus;
		}
	}

	clear();
	return status::fatal_error;
}

void shader_program::use() const {
	if (auto api{ controller::api() }; api) {
		api->use_program(mObject);
	}
}
void shader_program::unuse() const {
	if (auto api{ controller::api() }; api) {
		api->use_program(nullptr);
	}
}

core::u32 shader_program::id() const noexcept {
	if (mObject) {
		return mObject->id();
	}
	return ctrl::object::invalid_id;
}
shader_program::status shader_program::get_status() const noexcept {
	return mStatus;
}
bool shader_program::valid() const noexcept {
	return mObject != nullptr && mObject->valid();
}
ctrl::object::ref shader_program::to_object() const noexcept {
	return mObject;
}

bool shader_program::erase_shader(const ctrl::object::ref &shader_obj) noexcept {
	if (shader_obj == nullptr) {
		spdlog::warn("[{}] shader_obj is nullptr", class_name);
		return false;
	}

	auto end{ std::end(mAttachedShaders) };
	const auto shader_id{ shader_obj->id() };
	auto found{ std::remove_if(std::begin(mAttachedShaders), end,
		[shader_id](auto reference) { return reference->id() == shader_id; }) };
	if (found == end) {
		spdlog::warn("[{}] shader is not attached to the shader program", class_name);
		return false;
	}

	mAttachedShaders.erase(found, end);
	return true;
}

} // namespace golxzn::graphics::types
