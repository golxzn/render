#pragma once

#include "golxzn/graphics/types/shader.hpp"

namespace golxzn::graphics::types {

class shader_program {
	static constexpr const std::string_view class_name{ "graphics::types::shader_program" };
public:
	using ref = std::shared_ptr<shader_program>;

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

	static ref make();
	static ref make(std::initializer_list<shader> &&shaders);
	static ref make(std::initializer_list<shader::ref> &&shaders);

	shader_program();
	explicit shader_program(std::initializer_list<shader> &&shaders);
	explicit shader_program(std::initializer_list<shader::ref> &&shaders);

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

	core::u32 id() const noexcept;
	status get_status() const noexcept;
	bool valid() const noexcept;
	ctrl::object::ref to_object() const noexcept;

private:
	status mStatus{ status::invalid };
	ctrl::object::ref mObject;
	std::vector<shader::ref> mAttachedShaders;

	bool erase_shader(const ctrl::object::ref &shader_obj) noexcept;
};

} // namespace golxzn::graphics::types

namespace golxzn::graphics {
using program_status = types::shader_program::status;
} // namespace golxzn::graphics
