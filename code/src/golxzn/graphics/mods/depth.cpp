#include "golxzn/graphics/mods/depth.hpp"
#include "golxzn/graphics/controller/controller.hpp"

namespace golxzn::graphics::mods::depth {

void holder::set_mask(const bool flag) {
	mMask.value = flag;
}
void holder::set_function(const function func) {
	mFunctions.value = func;
}
void holder::set_range(const core::f16 near_value, const core::f16 far_value) {
	mRange.value = std::make_pair(near_value, far_value);
}

void holder::enable() {
	auto api{ controller::api() };
	if (api == nullptr) return;

	if (mMask.previous = api->get_depth_mask(); mMask.changed()) {
		api->set_depth_mask(mMask.value);
	}

	if (mFunctions.previous = api->get_depth_function(); mFunctions.changed()) {
		api->set_depth_function(mFunctions.value);
	}

	if (mRange.previous = api->get_depth_range(); mRange.changed()) {
		const auto [near_v, far_v]{ mRange.value };
		api->set_depth_range(near_v, far_v);
	}
}

void holder::disable() {
	auto api{ controller::api() };
	if (api == nullptr) return;

	if (mMask.changed()) {
		api->set_depth_mask(mMask.previous);
	}

	if (mFunctions.changed()) {
		api->set_depth_function(mFunctions.previous);
	}

	if (mRange.changed()) {
		const auto [near_v, far_v]{ mRange.previous };
		api->set_depth_range(near_v, far_v);
	}
}

} // golxzn::graphics::mods::depth
