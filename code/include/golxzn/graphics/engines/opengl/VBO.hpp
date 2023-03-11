#pragma once

#include <golxzn/graphics/engines/opengl/BaseBufferObject.hpp>

namespace golxzn::graphics::gl {

struct VBO : public BaseBufferObject<core::f16> {
	using base_type = BaseBufferObject<core::f16>;

	VBO() = default;
	explicit VBO(const std::vector<core::f16> &data) noexcept;

	void assign(const std::vector<core::f16> &data) noexcept;
	void bind() const noexcept;
	void unbind() const noexcept;
};

using VertexBufferObject = VBO;

} // namespace golxzn::graphics::gl
