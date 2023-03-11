#include <spdlog/spdlog.h>

#include "golxzn/graphics/window/window.hpp"
#include "golxzn/graphics/window/implementations/glfw_impl.hpp"
#include "golxzn/graphics/controller/controller.hpp"

namespace golxzn::graphics {

core::sptr<window::implementation> window::impl{ nullptr };

bool window::implementation::initialize(const core::u32 width, const core::u32 height, const std::string_view title) {
	mWidth = width;
	mHeight = height;
	mTitle = std::string{ title };
	return true;
}

void window::implementation::resize(const core::u32 width, const core::u32 height, const bool update_viewport) noexcept {
	mWidth = width;
	mHeight = height;
	if (!update_viewport) {
		return;
	}

	using namespace golxzn::types_literals;
	if (auto api{ controller::api() }; api) {
		api->viewport(0_u32, 0_u32, mWidth, mHeight);
	}
}

void window::implementation::set_title(const std::string_view title) noexcept {
	mTitle = std::string{ title };
}

bool window::initialize(const core::u32 width, const core::u32 height, const std::string_view title) noexcept {
	if (active()) {
		spdlog::warn("[{}]: Window already active", class_name);
		impl->resize(width, height);
		impl->set_title(title);
		return false;
	}

	impl = std::make_shared<glfw_impl>();
	return impl->initialize(width, height, title);
}
void window::destroy() noexcept {
	if (impl) {
		impl->destroy();
		impl.reset();
	}
}

bool window::active() noexcept {
	return impl != nullptr;
}

core::sptr<window::implementation> &window::api() noexcept {
	return impl;
}



} // namespace golxzn::graphics
