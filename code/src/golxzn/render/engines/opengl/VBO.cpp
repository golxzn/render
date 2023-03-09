#include <glad/gl.h>
#include <golxzn/render/engines/opengl/VBO.hpp>

namespace golxzn::render::gl {

namespace {
static constexpr core::u32 target{ GL_ARRAY_BUFFER };
} // anonymous namespace


VBO::VBO(const std::vector<core::f16> &data) noexcept
	: base_type{ data, target, GL_STATIC_DRAW } { }

void VBO::assign(const std::vector<core::f16> &data) noexcept {
	base_type::assign(data, target, GL_STATIC_DRAW);
}

void VBO::bind() const noexcept {
	base_type::bind(target);
}
void VBO::unbind() const noexcept {
	base_type::unbind(target);
}

} // namespace golxzn::render::gl
