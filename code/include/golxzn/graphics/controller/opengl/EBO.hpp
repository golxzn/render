#pragma once

#include <golxzn/graphics/controller/opengl/BaseBufferObject.hpp>

namespace golxzn::graphics::gl {

struct EBO : public BaseBufferObject<core::u32> {
	using base_type = BaseBufferObject<core::u32>;

	EBO() = default;
	explicit EBO(std::initializer_list<core::u32> &&data) noexcept;
	explicit EBO(const std::vector<core::u32> &data) noexcept;

	void assign(std::initializer_list<core::u32> &&data) noexcept;
	void assign(const std::vector<core::u32> &data) noexcept;
	void bind() const noexcept;
	void unbind() const noexcept;
};

using ElementBufferObject = EBO;

} // namespace golxzn::graphics::gl