#include <glad/gl.h>
#include <golxzn/graphics/engines/opengl/VBO.hpp>

namespace golxzn::graphics::gl {

namespace vbo_local {
static constexpr core::u32 target{ GL_ARRAY_BUFFER };
} // namespace


VBO::VBO(std::initializer_list<core::f16> &&data) noexcept
	: base_type{ std::move(data), vbo_local::target, GL_STATIC_DRAW } { }

VBO::VBO(const std::vector<core::f16> &data) noexcept
	: base_type{ data, vbo_local::target, GL_STATIC_DRAW } { }

void VBO::assign(std::initializer_list<core::f16> &&data) noexcept {
	base_type::assign(std::move(data), vbo_local::target, GL_STATIC_DRAW);
}
void VBO::assign(const std::vector<core::f16> &data) noexcept {
	base_type::assign(data, vbo_local::target, GL_STATIC_DRAW);
}

void VBO::bind() const noexcept {
	base_type::bind(vbo_local::target);
}
void VBO::unbind() const noexcept {
	base_type::unbind(vbo_local::target);
}

} // namespace golxzn::graphics::gl
