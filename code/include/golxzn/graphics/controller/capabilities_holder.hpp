#pragma once

#include <vector>
#include <string_view>

#include "golxzn/graphics/types/object.hpp"

namespace golxzn::graphics {

enum class capabilities;

class capabilities_holder {
public:
	static constexpr std::string_view parameter_name{ "capabilities" };
	using caps_container = std::vector<capabilities>;

	void set_target(types::object::ref object);
	void enable(const capabilities capability);
	void disable(const capabilities capability);
	bool is_enabled(const capabilities capability) const;

private:
	core::wptr<types::object> mObject;
};

} // namespace golxzn::graphics
