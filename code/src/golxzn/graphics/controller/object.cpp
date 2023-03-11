#include "golxzn/graphics/controller/object.hpp"

namespace golxzn::graphics::ctrl {

object::object(const core::u32 id) noexcept : mId{ id } { }

core::u32 object::id() const noexcept { return mId; }
bool object::valid() const noexcept { return mId != invalid_id; }

} // namespace golxzn::graphics::ctrl
