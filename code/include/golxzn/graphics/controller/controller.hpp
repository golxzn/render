#pragma once

#include <functional>

#include <golxzn/graphics/types/shader.hpp>
#include <golxzn/graphics/types/texture.hpp>
#include <golxzn/graphics/types/mesh.hpp>

namespace golxzn::graphics {

namespace mods {
enum class capabilities;

namespace blend {
enum class function;
enum class equation;
} // namespace blend

} // namespace mods


class controller {
public:
	GOLXZN_STATIC_CLASS(controller);

	class implementation {
	public:
		virtual ~implementation() = default;

		virtual bool initialize() = 0;
		virtual void destroy() = 0;

		virtual types::object::ref make_shader(const types::shader::type type, const std::string_view code) = 0;
		virtual types::object::ref make_program() = 0;
		virtual types::object::ref make_texture() = 0;
		virtual types::object::ref make_mesh(const std::vector<types::vertex> &vertices,
			const std::vector<core::u32> &indices) = 0;

		// Shader stuff
		virtual bool attach_shader(const types::object::ref &program, const types::object::ref &shader) = 0;
		virtual bool detach_shader(const types::object::ref &program, const types::object::ref &shader) = 0;

		// Program stuff
		virtual bool link_program(const types::object::ref &program) = 0;
		virtual void use_program(const types::object::ref &program) = 0;

		virtual void set_uniform(const types::object::ref &program, const std::string_view name,
			const std::any value, const std::type_info &info) = 0;

		// Texture stuff
		virtual bool make_texture_image_2d(types::object::ref texture, const core::bytes &data) = 0;

		virtual bool make_texture_image_2d(types::object::ref texture,
			const types::texture::cubemap_array<core::bytes> &data) = 0;

		virtual void generate_mip_maps(const types::object::ref &texture) = 0;

		virtual bool bind_texture(const types::object::ref &texture, const core::u32 unit) = 0;
		virtual bool unbind_texture(const types::object::ref &texture) = 0;

		template<class T, class V, std::enable_if_t<std::is_base_of_v<types::texture::parameter, T>, bool> = false>
		void set_texture_parameter(const types::object::ref &texture, const V value) {
			set_raw_texture_parameter(texture, typeid(T), value);
		}

		// Mesh stuff
		virtual void draw_mesh(const types::object::ref &mesh) = 0;

		virtual void viewport(const core::u32 x, const core::u32 y,
			const core::u32 width, const core::u32 height) noexcept = 0;

		virtual void enable(const mods::capabilities capability) = 0;
		virtual void enable(const mods::capabilities capability, const core::u32 value) = 0;
		virtual void disable(const mods::capabilities capability) = 0;
		virtual void disable(const mods::capabilities capability, const core::u32 value) = 0;
		virtual bool is_enabled(const mods::capabilities capability) const = 0;
		virtual bool is_enabled(const mods::capabilities capability, const core::u32 value) const = 0;
		virtual core::i32 capability_value(const mods::capabilities capability) const = 0;

		// Blend stuff
		virtual void set_blend_color(const glm::vec4 &color) noexcept = 0;
		virtual void unset_blend_color() noexcept = 0;

		virtual void set_blend_function(const mods::blend::function src, const mods::blend::function dest) = 0;
		virtual void unset_blend_function() = 0;

		virtual void set_blend_equation(const mods::blend::equation equation) = 0;
		virtual void unset_blend_equation() = 0;

		virtual void set_blend_function_separate(
			const mods::blend::function src_rgb,
			const mods::blend::function dest_rgb,
			const mods::blend::function src_alpha,
			const mods::blend::function dest_alpha) = 0;
		virtual void unset_blend_function_separate() = 0;

		virtual void set_blend_equation_separate(
			const mods::blend::equation equation_rgb,
			const mods::blend::equation equation_alpha) = 0;
		virtual void unset_blend_equation_separate() = 0;


	protected:
		virtual void set_raw_texture_parameter(const types::object::ref &texture,
			const std::type_info &param_type, const std::any param_value) = 0;
	};

	enum class api_type {
		opengl,
		vulkan,
		dx12,
	};
	static std::string_view api_type_to_str(const api_type type) noexcept;

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
