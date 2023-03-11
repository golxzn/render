#pragma once

#include <golxzn/core/types.hpp>

namespace golxzn {

template<class ...Extensions>
class extendable_render_base : public Extensions... {
public:
	using type = extendable_render_base<Extensions...>;

	template<class Extension>
	[[nodiscard]] static constexpr bool supports() noexcept {
		return std::is_base_of_v<Extension, type>;
	}

private:
};

} // namespace golxzn
