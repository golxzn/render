#include "golxzn/graphics/controller/object.hpp"

namespace golxzn::graphics::ctrl {

object::object(const core::u32 id) noexcept : mId{ id } { }
object::~object() {
	if (mDeleter) mDeleter(*this);
}

void object::set_deleter(deleter &&deleter) noexcept {
	mDeleter = std::move(deleter);
}

core::u32 object::id() const noexcept { return mId; }
bool object::valid() const noexcept { return mId != invalid_id; }

} // namespace golxzn::graphics::ctrl
