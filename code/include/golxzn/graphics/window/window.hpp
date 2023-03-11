#pragma once

#include <golxzn/core/types.hpp>

namespace golxzn::graphics {

class window {
	static constexpr std::string_view class_name{ "graphics::window" };
public:
	GOLXZN_STATIC_CLASS(window);

	class implementation {
	public:
		static constexpr core::u32 default_width{ 1280 };
		static constexpr core::u32 default_height{ 720 };
		static constexpr std::string_view default_title{ "golxzn" };

		virtual bool initialize(const core::u32 width, const core::u32 height,
			const std::string_view title = default_title);
		virtual void destroy() = 0;

		virtual bool should_close() const noexcept = 0;
		virtual void swap_buffers() noexcept = 0;

		virtual void resize(const core::u32 width, const core::u32 height,
			const bool update_viewport = true) noexcept;
		virtual void set_title(const std::string_view title) noexcept;

	protected:
		core::u32 mWidth{ default_width };
		core::u32 mHeight{ default_height };
		std::string mTitle{ std::string{ default_title } };
	};

	static bool initialize(
		const core::u32 width = implementation::default_width,
		const core::u32 height = implementation::default_height,
		const std::string_view title = implementation::default_title) noexcept;
	static void destroy() noexcept;
	static bool active() noexcept;

	static core::sptr<implementation> &api() noexcept;

private:
	// using impl_maker = std::function<core::sptr<implementation>()>;

	static core::sptr<implementation> impl;
	// static core::umap<api_type, impl_maker> api_initializer;
};

} // namespace golxzn::graphics
