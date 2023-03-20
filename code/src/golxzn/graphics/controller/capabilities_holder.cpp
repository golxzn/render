#include "golxzn/graphics/controller/capabilities_holder.hpp"
#include "golxzn/graphics/controller/controller.hpp"

namespace golxzn::graphics {

void capabilities_holder::set_target(types::object::ref object) {
	mObject = object;
}
void capabilities_holder::enable(const capabilities capability) {
	auto object{ mObject.lock() };
	if (object == nullptr || is_enabled(capability)) return;

	auto caps{ object->get_property<caps_container>(parameter_name).value_or(caps_container{}) };
	caps.emplace_back(capability);
	object->set_property(parameter_name, std::move(caps));
}

void capabilities_holder::disable(const capabilities capability) {
	auto object{ mObject.lock() };
	if (object == nullptr || !is_enabled(capability)) return;

	auto caps{ object->get_property<caps_container>(parameter_name).value_or(caps_container{}) };
	caps.erase(
		std::remove(std::begin(caps), std::end(caps), capabilities::depth_test),
		std::end(caps)
	);
	object->set_property(parameter_name, std::move(caps));
}

bool capabilities_holder::is_enabled(const capabilities capability) const {
	auto object{ mObject.lock() };
	if (object == nullptr) return false;

	auto caps{ object->get_property<caps_container>(parameter_name).value_or(caps_container{}) };
	return std::find(std::begin(caps), std::end(caps), capability) != std::end(caps);
}

} // namespace golxzn::graphics
