#pragma once

#include "golxzn/graphics/types/shader/shader.hpp"
#include "golxzn/graphics/controller/controller.hpp"

namespace golxzn::graphics::types {

class shader_program : named {
	static constexpr const std::string_view class_name{ "graphics::types::shader_program" };
public:
	using ref = core::sptr<shader_program>;

	enum class status : core::u8 {
		invalid,
		fatal_error,

		need_to_link,
		link_success,
		link_failure,

		attach_success,
		attach_failure,
		attach_already,

		detach_success,
		detach_failure,
	};

	static ref make(const std::string &name);
	static ref make(const std::string &name, std::initializer_list<shader> &&shaders);
	static ref make(const std::string &name, std::initializer_list<shader::ref> &&shaders);
	static ref make(const std::string &name, std::initializer_list<std::string> &&shaders);

	shader_program(const std::string &name);
	explicit shader_program(const std::string &name, std::initializer_list<shader> &&shaders);
	explicit shader_program(const std::string &name, std::initializer_list<shader::ref> &&shaders);
	explicit shader_program(const std::string &name, std::initializer_list<std::string> &&shaders);

	void clear() noexcept;

	bool is_attached(const shader &shader_obj) const noexcept;
	bool is_attached(const shader::ref &shader_obj) const noexcept;

	status attach(shader &&shader_obj) noexcept;
	status attach(const shader &shader_obj) noexcept;
	status attach(shader::ref shader_ref) noexcept;

	status detach(const shader &shader_obj) noexcept;
	status detach(shader::ref shader_ref) noexcept;

	status link() noexcept;

	void use() const;
	void unuse() const;

	template<class T>
	void set_uniform(const std::string_view name, const T &value) const;

	object::id_t id() const noexcept;
	status get_status() const noexcept;
	bool valid() const noexcept;
	object::ref to_object() const noexcept;

private:
	status mStatus{ status::invalid };
	object::ref mObject;
	std::vector<shader::ref> mAttachedShaders;

	bool erase_shader(const object::ref &shader_obj) noexcept;
};

template<class T>
void shader_program::set_uniform(const std::string_view name, const T &value) const {
	if (auto api{ controller::api() }; api) {
		api->set_uniform(mObject, name, value, typeid(T));
	}
}

} // namespace golxzn::graphics::types

namespace golxzn::graphics {
using program_status = types::shader_program::status;
} // namespace golxzn::graphics

