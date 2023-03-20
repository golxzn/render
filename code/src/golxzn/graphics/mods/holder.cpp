#include "golxzn/graphics/mods/holder.hpp"

namespace golxzn::graphics::mods {

void holder::enable_mods() {
	for (auto&& [key, mods] : mmodss) mods->enable();
}

void holder::disable_mods() {
	for (auto&& [key, mods] : mmodss) mods->disable();
}

} // namespace golxzn::graphics::mods
