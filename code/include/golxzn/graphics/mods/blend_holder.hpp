#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <optional>
#include <string_view>

#include <golxzn/core/utils/traits.hpp>
#include "golxzn/graphics/mods/holder.hpp"

namespace golxzn::graphics::mods::blend {

enum class function {
	zero,
	one,
	src_color,
	one_minus_src_color,
	dst_color,
	one_minus_dst_color,
	src_alpha,
	one_minus_src_alpha,
	dst_alpha,
	one_minus_dst_alpha,
	constant_color,
	one_minus_constant_color,
	constant_alpha,
	one_minus_constant_alpha,
	src_alpha_saturate,
	src1_color,
	one_minus_src1_color,
	src1_alpha,
	one_minus_src1_alpha,
};

enum class equation {
	add,
	subtract,
	reverse_subtract,
	min,
	max,
};

template<class T, std::enable_if_t<core::traits::is_any_of_v<T, function, equation>, bool> = false>
struct separate {
	union{ T source; T rgb; };
	union{ T destination; T alpha; };
};

class holder : public mod_interface {
public:
	void set_color(const glm::vec4 color);
	void unset_color();

	void set_equation(const equation mode);
	void unset_equation();

	void set_equation_separate(const equation mode_rgb, const equation mode_alpha);
	void unset_equation_separate();

	void set_function(const function src = function::one, const function dst = function::zero);
	void unset_function();

	void set_function_separate(const function src_rgb = function::one, const function dst_rgb = function::zero,
		const function src_alpha = function::one, const function dst_alpha = function::zero);
	void unset_function_separate();

	void enable() override;
	void disable() override;

private:
	std::optional<glm::vec4> mColor;
	std::optional<equation> mEquation;
	std::optional<separate<equation>> mEquationSeparate;
	std::optional<separate<function>> mFunction;
	/// @todo: remove pair and make it as separate<separate<functino>>
	std::optional<std::pair<separate<function>, separate<function>>> mFunctionSeparate;
};

} // namespace golxzn::graphics::mods::blend
