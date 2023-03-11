#include "golxzn/graphics/controller/controller.hpp"

#include "golxzn/graphics/controller/implementations/gl_impl.hpp"

namespace golxzn::graphics {

template<class impl, std::enable_if_t<
	std::is_base_of_v<controller::implementation, impl> ||
	std::is_same_v<std::nullptr_t, impl>, bool> = false>
core::sptr<controller::implementation> make_impl() { return std::make_shared<impl>(); }

template<> core::sptr<controller::implementation> make_impl<std::nullptr_t>() { return nullptr; }


core::sptr<controller::implementation> controller::impl{ nullptr };
core::umap<controller::api_type, controller::impl_maker> controller::api_initializer {
	{ api_type::opengl,     &make_impl<gl_impl> },
	{ api_type::vulkan,     &make_impl<std::nullptr_t> },
	{ api_type::dx12,       &make_impl<std::nullptr_t> },
};


bool controller::initialize(const api_type render_api) noexcept {
	destroy();

	if (const auto found{ api_initializer.find(render_api) }; found != std::end(api_initializer)) {
		impl = found->second();
		if (const bool success{ impl->initialize() }; success) {
			return success;
		}
		destroy();
	}
	return active();
}

void controller::destroy() noexcept {
	if (active()) {
		impl->destroy();
		impl.reset();
	}
}


bool controller::active() noexcept {
	return impl != nullptr;
}

core::sptr<controller::implementation> &controller::api() noexcept {
	return impl;
}

} // namespace golxzn::graphics
