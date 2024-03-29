#pragma once

#include <glm/glm.hpp>
#include "golxzn/graphics/types/object.hpp"
#include "golxzn/graphics/types/name_manager.hpp"
#include "golxzn/graphics/mods/capabilities.hpp"

namespace golxzn::graphics::types {

struct material;
class texture;
class shader_program;
enum class draw_mode;

struct vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 UV;
};

class mesh : public named, public mods::holder {
	static constexpr std::string_view class_name{ "graphics::types::mesh" };
public:
	using ref = core::sptr<mesh>;

	static ref make(const std::string &name, const std::vector<vertex> &vertices, const std::vector<core::u32> &indices,
		const core::sptr<shader_program> &shader_program = {},
		const core::sptr<material> &material = {},
		const core::umap<std::string, core::sptr<texture>> &textures = {});

	static ref make(const std::string &name, std::vector<vertex> &&vertices, std::vector<core::u32> &&indices,
		const core::sptr<shader_program> &shader_program = {},
		const core::sptr<material> &material = {},
		const core::umap<std::string, core::sptr<texture>> &textures = {});


	mesh(const std::string &name, const std::vector<vertex> &vertices, const std::vector<core::u32> &indices,
		const core::sptr<shader_program> &shader_program = {},
		const core::sptr<material> &material = {},
		const core::umap<std::string, core::sptr<texture>> &textures = {});

	mesh(const std::string &name, std::vector<vertex> &&vertices, std::vector<core::u32> &&indices,
		const core::sptr<shader_program> &shader_program = {},
		const core::sptr<material> &material = {},
		const core::umap<std::string, core::sptr<texture>> &textures = {});

	bool valid() const noexcept;

	void set_shader_program(const core::sptr<shader_program> &shader_program);
	void set_vertices(std::vector<vertex> &&vertices, std::vector<core::u32> &&indices);
	void set_material(const core::sptr<material> &material);
	void add_texture(const std::string &name, core::sptr<texture> texture);
	void remove_texture(const std::string &name);
	void set_draw_mode(const draw_mode &mode);

	void draw();

	const std::vector<vertex> &get_vertices() const;
	const std::vector<core::u32> &get_indices() const;
	core::sptr<shader_program> get_shader_program();
	core::sptr<material> get_material();
	const core::umap<std::string, core::sptr<texture>> &get_textures() const;
	draw_mode get_draw_mode() const;

private:
	object::ref mObject;
	std::vector<vertex> mVertices;
	std::vector<core::u32> mIndices;
	core::sptr<shader_program> mShaderProgram;
	core::sptr<material> mMaterial;
	core::umap<std::string, core::sptr<texture>> mTextures;

	void generate();
	void update_shader_program_uniforms();
};

enum class draw_mode {
	points,
	line_strip,
	line_loop,
	lines,
	line_strip_adjacency,
	lines_adjacency,
	triangle_strip,
	triangle_fan,
	triangles,
	triangle_strip_adjacency,
	triangles_adjacency,
};

} // namespace golxzn::graphics::types
