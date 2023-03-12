#pragma once

#include "golxzn/graphics/types/object.hpp"

namespace golxzn::graphics::types {

class shader {
	static constexpr std::string_view class_name{ "graphics::types::shader" };
public:
	using ref = core::sptr<shader>;

	enum class type : core::u8 {
		invalid,

		vertex,                 ///< shader_name.vs.glsl file
		tesselation_control,    ///< shader_name.tcs.glsl file
		tesselation_evaluation, ///< shader_name.tes.glsl file
		geometry,               ///< shader_name.gs.glsl file
		fragment,               ///< shader_name.fs.glsl file
		compute,                ///< shader_name.cs.glsl file

		count // always last
	};
	enum class status : core::u8 {
		invalid,

		need_to_compile,
		compile_success,
		compile_failure,

		count // always last
	};

	static ref make(const std::string_view path);
	static ref make(std::string &&code, const type shader_type);

	shader() = default;
	shader(const shader &other);
	shader(shader &&other) noexcept;

	explicit shader(const std::string_view path);
	shader(std::string &&code, const type shader_type);
	~shader();

	shader &operator=(const shader &other);
	shader &operator=(shader &&other) noexcept;

	status compile();
	void clear() noexcept;

	core::u32 id() const noexcept;
	type get_type() const noexcept;
	status get_status() const noexcept;
	const std::string &get_code() const noexcept;
	types::object::ref to_object() const noexcept;

	status set_code(std::string &&code);
	status set_code(std::string &&code, const type shader_type);

	bool valid() const noexcept;

	static type get_type_by_extension(const std::string_view filename) noexcept;
	static std::string_view get_extension(const type type) noexcept;
	static std::string_view type_to_str(const shader::type type) noexcept;

private:
	type mType{ type::invalid };
	status mStatus{ status::invalid };
	std::string mCode;
	types::object::ref mObject;
};

} // namespace golxzn::graphics::types
