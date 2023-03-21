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

	constexpr separate(const T source, const T destination) noexcept
		: source{ source }, destination{ destination } {}

	constexpr explicit separate(const std::pair<T, T> &pair) noexcept
		: source{ pair.first }, destination{ pair.second } {}

	separate &operator=(const std::pair<T, T> &pair) noexcept {
		source = pair.first;
		destination = pair.second;
		return *this;
	}

	constexpr bool operator==(const separate &other) const noexcept {
		return source == other.source && destination == other.destination;
	}
	constexpr bool operator!=(const separate &other) const noexcept {
		return !(*this == other);
	}
};

class holder : public mod_interface {
public:
	static constexpr glm::vec4 default_color{ 0.0f };
	static constexpr equation default_equation{ equation::add };
	static constexpr separate<equation> default_equation_separate{ equation::add, equation::add };
	static constexpr separate<function> default_function{ function::one, function::zero };
	static constexpr std::pair<separate<function>, separate<function>> default_function_separate{
		default_function, default_function
	};

	void set_color(const glm::vec4 color);
	void set_equation(const equation mode);
	void set_equation_separate(const equation mode_rgb, const equation mode_alpha);
	void set_function(const function src = function::one, const function dst = function::zero);
	void set_function_separate(const function src_rgb = function::one, const function dst_rgb = function::zero,
		const function src_alpha = function::one, const function dst_alpha = function::zero);

	void enable() override;
	void disable() override;

private:
	cache_value<glm::vec4> mColor{ default_color };
	cache_value<equation> mEquation{ default_equation };
	cache_value<separate<equation>> mEquationSeparate{ default_equation_separate };
	cache_value<separate<function>> mFunction{ default_function };
	/// @todo: remove pair and make it as separate<separate<functino>>
	cache_value<std::pair<separate<function>, separate<function>>> mFunctionSeparate{ default_function_separate };
};

} // namespace golxzn::graphics::mods::blend
