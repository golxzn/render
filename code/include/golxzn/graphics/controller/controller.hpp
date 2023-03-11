#pragma once

#include <functional>

#include <golxzn/graphics/controller/object.hpp>

namespace golxzn::graphics {

class controller {
public:
	GOLXZN_STATIC_CLASS(controller);

	class implementation {
	public:
		virtual ~implementation() = default;

		virtual bool initialize() = 0;
		virtual void destroy() = 0;

		virtual ctrl::object::ref make_shader(const std::string_view type, const std::string_view code) = 0;
		virtual ctrl::object::ref make_program() = 0;
		virtual ctrl::object::ref make_texture(const std::string &path) = 0;

		virtual void viewport(const core::u32 x, const core::u32 y,
			const core::u32 width, const core::u32 height) noexcept = 0;
	};

	enum class api_type {
		opengl,
		vulkan,
		dx12,
	};

	static bool initialize(const api_type render_api) noexcept;
	static void destroy() noexcept;
	static bool active() noexcept;

	static core::sptr<implementation> &api() noexcept;

private:
	using impl_maker = std::function<core::sptr<implementation>()>;

	static core::sptr<implementation> impl;
	static core::umap<api_type, impl_maker> api_initializer;
};

} // namespace golxzn::graphics
