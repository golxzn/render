#pragma once

#include <golxzn/render/engines/opengl/BaseBufferObject.hpp>

namespace golxzn::render::gl {

struct EBO : public BaseBufferObject<core::u32> {
	using base_type = BaseBufferObject<core::u32>;

	EBO() = default;
	explicit EBO(const std::vector<core::u32> &data) noexcept;

	void assign(const std::vector<core::u32> &data) noexcept;
	void bind() const noexcept;
	void unbind() const noexcept;
};

using ElementBufferObject = EBO;

} // namespace golxzn::render::gl