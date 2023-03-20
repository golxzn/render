#pragma once

#include <golxzn/core/types.hpp>

namespace golxzn::graphics::mods {

struct mod_interface {
	virtual ~mod_interface() = default;
	virtual void enable() = 0;
	virtual void disable() = 0;
};

class holder {
public:
	virtual ~holder() = default;

	template<class T, std::enable_if_t<std::is_base_of_v<mod_interface, T>, bool> = false>
	core::sptr<T> add_mod() {
		if (auto found{ mmodss.find(typeid(T).name()) }; found != std::end(mmodss)) {
			return std::static_pointer_cast<T>(found->second);
		}
		if (auto mods{ std::make_shared<T>() }; mods != nullptr) {
			mmodss.emplace(typeid(T).name(), mods);
			return mods;
		}
		return nullptr;
	}

	template<class T, std::enable_if_t<std::is_base_of_v<mod_interface, T>, bool> = false>
	core::sptr<T> get_mod() {
		return add_mod<T>();
	}

	template<class T, std::enable_if_t<std::is_base_of_v<mod_interface, T>, bool> = false>
	void erase_mod() {
		mmodss.erase(typeid(T).name());
	}

	void enable_mods();
	void disable_mods();

private:
	core::umap<std::string, core::sptr<mod_interface>> mmodss;
};

} // namespace golxzn::graphics::mods
