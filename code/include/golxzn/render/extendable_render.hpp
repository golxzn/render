#pragma once

#include <golxzn/core/types.hpp>

namespace golxzn {

template<class RenderEngine, class ...Extensions>
class extendable_render_base : public Extensions<RenderEngine>... {
public:
	using engine = RenderEngine;
	using type = extendable_render_base<RenderEngine, Extensions...>;

	static bool initialize() noexcept(noexcept(engine::initialize())) {
		return engine::initialize();
	}

	template<class Extension>
	static constexpr bool supports() const noexcept {
		return std::is_base_of_v<Extension, type>;
	}

private:
};

} // namespace golxzn
