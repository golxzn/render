#pragma once

#include <vector>
#include <golxzn/core/types.hpp>

namespace golxzn::render::gl {

core::u32 generate_buffer() noexcept;
void destroy_buffer(core::u32 id) noexcept;
void assign_buffer(core::u32 id, core::u32 target, std::size_t size, const void *data, core::u32 usage) noexcept;
void bind_buffer(core::u32 id, core::u32 target) noexcept;

template<class T>
class BaseBufferObject {
public:
	using value_type = T;
	static constexpr core::u32 invalid_id{ 0 };

	BaseBufferObject() = default;
	explicit BaseBufferObject(const std::vector<T> &data, const core::u32 target, const core::u32 usage) noexcept {
		assign(data, target, usage);
	}

	BaseBufferObject(const BaseBufferObject &other) = delete;
	BaseBufferObject &operator=(const BaseBufferObject &other) = delete;

	BaseBufferObject(BaseBufferObject &&other) noexcept : mId{ other.mId } {
		other.mId = invalid_id;
	}
	BaseBufferObject &operator=(BaseBufferObject &&other) noexcept {
		if (this != &other) {
			clean();
			mId = other.mId;
			other.mId = invalid_id;
		}
		return *this;
	}

	void clean() noexcept {
		destroy_buffer(mId);
		mId = invalid_id;
	}

	bool valid() const noexcept {
		return mId != invalid_id;
	}

protected:
	void assign(const std::vector<T> &data, const core::u32 target, const core::u32 usage) noexcept {
		if (valid()) {
			clean();
		}
		mId = generate_buffer();
		bind(target);
		assign_buffer(mId, target, data.size() * sizeof(T), data.data(), usage);
		unbind(target);
	}
	void bind(const core::u32 target) const noexcept {
		bind_buffer(mId, target);
	}
	void unbind(const core::u32 target) const noexcept {
		bind_buffer(invalid_id, target);
	}
	core::u32 mId{ invalid_id };
};

} // namespace golxzn::render::gl
