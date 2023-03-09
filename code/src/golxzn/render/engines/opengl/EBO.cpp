#include <glad/gl.h>
#include "golxzn/render/engines/opengl/EBO.hpp"

namespace golxzn::render::gl {

namespace {
static constexpr core::u32 target{ GL_ELEMENT_ARRAY_BUFFER };
} // anonymous namespace

EBO::EBO(const std::vector<core::u32> &data) noexcept
	: base_type{ data, target, GL_STATIC_DRAW } {
	bind();
}

void EBO::assign(const std::vector<core::u32> &data) noexcept {
	base_type::assign(data, target, GL_STATIC_DRAW);
	bind();
}

void EBO::bind() const noexcept {
	base_type::bind(target);
}
void EBO::unbind() const noexcept {
	base_type::unbind(target);
}

} // namespace golxzn::render::gl
