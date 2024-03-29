#pragma once

#include <golxzn/graphics/controller/controller.hpp>
#include <golxzn/graphics/extendable_render.hpp>
#include <golxzn/graphics/extensions.hpp>

namespace golxzn {

using render = extendable_render_base<
	graphics::extensions::primitives,
	graphics::extensions::ext_gizmos
>;

} // namespace golxzn
