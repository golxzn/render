#pragma once

#include "golxzn/graphics/mods/holder.hpp"

namespace golxzn::graphics::mods {

enum class capabilities {
	blend,
	clip_distance,
	color_logic_op,
	cull_face,
	debug_output,
	debug_output_synchronous,
	depth_clamp,
	depth_test,
	dither,
	framebuffer_srgb,
	line_smooth,
	multisample,
	polygon_offset_fill,
	polygon_offset_line,
	polygon_offset_point,
	polygon_smooth,
	primitive_restart,
	primitive_restart_fixed_index,
	rasterizer_discard,
	sample_alpha_to_coverage,
	sample_alpha_to_one,
	sample_coverage,
	sample_shading,
	sample_mask,
	scissor_test,
	stencil_test,
	texture_cube_map_seamless,
	program_point_size,
};

class mod_capabilities final : public mod_interface {
public:
	~mod_capabilities() override = default;

	void set(const capabilities capability);
	void reset(const capabilities capability);
	bool is_set(const capabilities capability) const noexcept;

	void enable() override;
	void disable() override;

private:
	core::uset<capabilities> mCapabilities;
	core::umap<capabilities, bool> mStates;
};


} // namespace golxzn::graphics::mods
