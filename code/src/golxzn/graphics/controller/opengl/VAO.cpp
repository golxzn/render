#include <glad/gl.h>
#include "golxzn/graphics/controller/opengl/VAO.hpp"

namespace golxzn::graphics::gl {

VAO::VAO() noexcept {
	glGenVertexArrays(1, &mId);
}

void VAO::link_attribute(const VBO &vbo, const core::u32 layout, const core::u32 components_count,
	const core::u32 type, const core::u32 stride, const void *offset) const noexcept {
	if (!valid()) {
		return;
	}

	vbo.bind();

	glVertexAttribPointer(layout, components_count, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);

	vbo.unbind();
}

void VAO::bind() const noexcept {
	glBindVertexArray(mId);
}
void VAO::unbind() const noexcept {
	glBindVertexArray(invalid_id);
}
void VAO::clean() noexcept {
	glDeleteVertexArrays(1, &mId);
	mId = invalid_id;
}

bool VAO::valid() const noexcept {
	return mId != invalid_id;
}

} // namespace golxzn::graphics::gl
