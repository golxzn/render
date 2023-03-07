#pragma once

#include <golxzn/render/extendable_render.hpp>
#include <golxzn/render/engines/opengl.hpp>
#include <golxzn/render/extensions.hpp>

namespace golxzn {

template<class ...Extensions>
using opengl_render_base = extendable_render_base<
	render::engines::opengl,
	Extensions
>;

using opengl_render = opengl_render_base<
	render::extensions::primitives
>;

using render = opengl_render;

} // namespace golxzn
