#include "golxzn/graphics/mods/capabilities.hpp"
#include "golxzn/graphics/controller/controller.hpp"

namespace golxzn::graphics::mods {

void mod_capabilities::set(const capabilities capability) {
	mCapabilities.emplace(capability);
}

void mod_capabilities::reset(const capabilities capability) {
	mCapabilities.erase(capability);
}

bool mod_capabilities::is_set(const capabilities capability) const noexcept {
	return mCapabilities.find(capability) != std::end(mCapabilities);
}

void mod_capabilities::enable() {
	if (auto api{ controller::api() }; api != nullptr) {
		for (const auto capability : mCapabilities) {
			const bool enabled{ api->is_enabled(capability) };
			mStates.insert_or_assign(capability, enabled);
			if (!enabled) {
				api->enable(capability);
			}
		}
	}
}

void mod_capabilities::disable() {
	if (auto api{ controller::api() }; api != nullptr) {
		for (const auto capability : mCapabilities) {
			const auto found{ mStates.find(capability) };
			// if it wasn't enabled before this mod was applied,
			// then we disable it cuz we enabled it.
			if (!found->second) {
				api->disable(capability);
			}
		}
	}
}

} // namespace golxzn::graphics::mods
