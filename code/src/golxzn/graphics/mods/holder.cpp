#include "golxzn/graphics/mods/holder.hpp"

namespace golxzn::graphics::mods {

void holder::enable_mods() {
	for (auto&& [key, mods] : mMods) mods->enable();
}

void holder::disable_mods() {
	for (auto&& [key, mods] : mMods) mods->disable();
}

} // namespace golxzn::graphics::mods
