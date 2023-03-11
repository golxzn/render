#pragma once

#include <golxzn/graphics/engines/opengl/VBO.hpp>

namespace golxzn::graphics::gl {

class VAO final {
public:
	static constexpr core::u32 invalid_id{ 0 };

	VAO() noexcept;

	void link_attribute(const VBO &vbo,
		const core::u32 layout,
		const core::u32 components_count,
		const core::u32 type,
		const core::u32 stride,
		const void *offset) const noexcept;

	void bind() const noexcept;
	void unbind() const noexcept;
	void clean() noexcept;

	bool valid() const noexcept;
private:
	core::u32 mId{ invalid_id };
};

} // namespace golxzn::graphics::gl