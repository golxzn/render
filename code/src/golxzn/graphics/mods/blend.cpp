#include "golxzn/graphics/mods/blend.hpp"
#include "golxzn/graphics/controller/controller.hpp"

namespace golxzn::graphics::mods::blend {

void holder::set_color(const glm::vec4 color) {
	mColor.value = color;
}

void holder::set_equation(const equation mode) {
	mEquation.value = mode;
}

void holder::set_equation_separate(const equation mode_rgb, const equation mode_alpha) {
	mEquationSeparate.value = separate<equation>{ mode_rgb, mode_alpha };
}

void holder::set_function(const function src, const function dst) {
	mFunction.value = separate<function>{ src, dst };
}

void holder::set_function_separate(const function src_rgb, const function dst_rgb,
		const function src_alpha, const function dst_alpha) {
	mFunctionSeparate.value = std::make_pair(
		separate<function>{ src_rgb, dst_rgb },
		separate<function>{ src_alpha, dst_alpha }
	);
}

void holder::enable() {
	auto api{ controller::api() };
	if (api == nullptr) return;

	if (mColor.previous = api->get_blend_color(); mColor.changed()) {
		api->set_blend_color(mColor.value);
	}
	if (mEquation.previous = api->get_blend_equation(); mEquation.changed()) {
		api->set_blend_equation(mEquation.value);
	} else if (mEquationSeparate.previous = api->get_blend_equation_separate(); mEquationSeparate.changed()) {
		api->set_blend_equation_separate(mEquationSeparate.value.rgb, mEquationSeparate.value.alpha);
	}
	if (mFunction.previous = api->get_blend_function(); mFunction.changed()) {
		api->set_blend_function(mFunction.value.source, mFunction.value.destination);
		return;
	}

	const auto [src_rgb, src_alpha, dst_rgb, dst_alpha]{ api->get_blend_function_separate() };
	mFunctionSeparate.previous = std::make_pair(
		separate<function>{ src_rgb, dst_rgb },
		separate<function>{ src_alpha, dst_alpha }
	);
	if (mFunctionSeparate.changed()) {
		const auto &[rgb, alpha] = mFunctionSeparate.value;
		api->set_blend_function_separate(rgb.source, rgb.destination, alpha.source, alpha.destination);
	}
}
void holder::disable() {
	auto api{ controller::api() };
	if (api == nullptr) return;

	if (mColor.changed()) {
		api->set_blend_color(mColor.previous);
	}
	if (mEquation.changed()) {
		api->set_blend_equation(mEquation.previous);
	}
	if (mEquationSeparate.changed()) {
		api->set_blend_equation_separate(
			mEquationSeparate.previous.rgb, mEquationSeparate.previous.alpha);
	}
	if (mFunction.changed()) {
		api->set_blend_function(mFunction.previous.source, mFunction.previous.destination);
	} else if (mFunctionSeparate.changed()) {
		const auto &[rgb, alpha] = mFunctionSeparate.previous;
		api->set_blend_function_separate(rgb.source, rgb.destination, alpha.source, alpha.destination);
	}
}

} // namespace golxzn::graphics::mods::blend

