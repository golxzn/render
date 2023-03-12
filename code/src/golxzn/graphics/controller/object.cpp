#include "golxzn/graphics/types/object.hpp"

namespace golxzn::graphics::types {

object::ref object::make(const core::u32 id, deleter &&deleter) {
	return std::make_shared<object>(id, std::move(deleter));
}

object::object(const core::u32 id, deleter &&deleter) noexcept
	: mId{ id }, mDeleter{ std::move(deleter) } { }

object::~object() {
	if (mDeleter) mDeleter(*this);
}

void object::set_deleter(deleter &&deleter) noexcept {
	mDeleter = std::move(deleter);
}

core::u32 object::id() const noexcept { return mId; }
bool object::valid() const noexcept { return mId != invalid_id; }

} // namespace golxzn::graphics::types
