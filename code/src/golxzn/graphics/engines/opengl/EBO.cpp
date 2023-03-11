#include <glad/gl.h>
#include "golxzn/graphics/engines/opengl/EBO.hpp"

namespace golxzn::graphics::gl {

namespace ebo_local {
static constexpr core::u32 target{ GL_ELEMENT_ARRAY_BUFFER };
} // namespace ebo_local

EBO::EBO(const std::vector<core::u32> &data) noexcept
	: base_type{ data, ebo_local::target, GL_STATIC_DRAW } {
	bind();
}

void EBO::assign(const std::vector<core::u32> &data) noexcept {
	base_type::assign(data, ebo_local::target, GL_STATIC_DRAW);
	bind();
}

void EBO::bind() const noexcept {
	base_type::bind(ebo_local::target);
}
void EBO::unbind() const noexcept {
	base_type::unbind(ebo_local::target);
}

} // namespace golxzn::graphics::gl
