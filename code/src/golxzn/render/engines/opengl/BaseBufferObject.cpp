#include <glad/gl.h>
#include "golxzn/render/engines/opengl/BaseBufferObject.hpp"

namespace golxzn::render::gl {

core::u32 generate_buffer() noexcept {
	core::u32 id;
	glGenBuffers(1, &id);
	return id;
}
void destroy_buffer(core::u32 id) noexcept {
	glDeleteBuffers(1, &id);
}
void assign_buffer(core::u32 id, core::u32 target, std::size_t size, const void *data, core::u32 usage) noexcept {
	glBufferData(target, size, data, usage);
}
void bind_buffer(core::u32 id, core::u32 target) noexcept {
	glBindBuffer(target, id);
}


} // namespace golxzn::render::gl