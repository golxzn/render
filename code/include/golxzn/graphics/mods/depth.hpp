#pragma once

#include "golxzn/graphics/mods/holder.hpp"

namespace golxzn::graphics::mods::depth {

enum class function {
	never,
	less,
	equal,
	lequal,
	greater,
	not_equal,
	gequal,
	always,
};

class holder final : public mod_interface {
public:
	~holder() override = default;

	void set_mask(const bool flag);
	void set_function(const function func = function::less);
	void set_range(const core::f16 near_value = 0.0f, const core::f16 far_value = 1.0f);

	void enable() override;
	void disable() override;

private:
	cache_value<bool> mMask{ false };
	cache_value<function> mFunctions{ function::less };
	cache_value<std::pair<core::f16, core::f16>> mRange{ std::make_pair(0.0f, 1.0f) };
};


} // namespace golxzn::graphics::mods::depth

