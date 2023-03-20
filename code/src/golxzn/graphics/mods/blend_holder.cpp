#include "golxzn/graphics/mods/blend_holder.hpp"
#include "golxzn/graphics/controller/controller.hpp"

namespace golxzn::graphics::mods::blend {

void holder::set_color(const glm::vec4 color) {
	mColor = color;
}
void holder::unset_color() {
	mColor.reset();
}

void holder::set_equation(const equation mode) {
	mEquation = mode;
}
void holder::unset_equation() {
	mEquation.reset();
}

void holder::set_equation_separate(const equation mode_rgb, const equation mode_alpha) {
	mEquationSeparate = separate<equation>{ mode_rgb, mode_alpha };
}
void holder::unset_equation_separate() {
	mEquationSeparate.reset();
}

void holder::set_function(const function src, const function dst) {
	mFunction = separate<function>{ src, dst };
}
void holder::unset_function() {
	mFunction.reset();
}

void holder::set_function_separate(const function src_rgb, const function dst_rgb,
		const function src_alpha, const function dst_alpha) {
	mFunctionSeparate = std::make_pair(
		separate<function>{ src_rgb, dst_rgb },
		separate<function>{ src_alpha, dst_alpha }
	);
}
void holder::unset_function_separate() {
	mFunctionSeparate.reset();
}

void holder::enable() {
	auto api{ controller::api() };
	if (api == nullptr) return;

	if (mColor.has_value()) {
		api->set_blend_color(mColor.value());
	}
	if (mEquation.has_value()) {
		api->set_blend_equation(mEquation.value());
	}
	if (mEquationSeparate.has_value()) {
		const auto equat{ mEquationSeparate.value() };
		api->set_blend_equation_separate(equat.rgb, equat.alpha);
	}
	if (mFunction.has_value()) {
		const auto func{ mFunction.value() };
		api->set_blend_function(func.source, func.destination);
	}
	if (mFunctionSeparate.has_value()) {
		const auto [source, destination]{ mFunctionSeparate.value() };
		api->set_blend_function_separate(
			source.rgb, destination.rgb,
			source.alpha, destination.alpha
		);
	}
}
void holder::disable() {
	auto api{ controller::api() };
	if (api == nullptr) return;

	if (mColor.has_value()) {
		api->unset_blend_color();
	}
	if (mEquation.has_value()) {
		api->unset_blend_equation();
	}
	if (mEquationSeparate.has_value()) {
		api->unset_blend_equation_separate();
	}
	if (mFunction.has_value()) {
		api->unset_blend_function();
	}
	if (mFunctionSeparate.has_value()) {
		api->unset_blend_function_separate();
	}
}

} // namespace golxzn::graphics::mods::blend

