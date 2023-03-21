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
		if (auto found{ mMods.find(typeid(T).name()) }; found != std::end(mMods)) {
			return std::static_pointer_cast<T>(found->second);
		}
		if (auto mods{ std::make_shared<T>() }; mods != nullptr) {
			mMods.emplace(typeid(T).name(), mods);
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
		mMods.erase(typeid(T).name());
	}

	void enable_mods();
	void disable_mods();

private:
	core::umap<std::string, core::sptr<mod_interface>> mMods;
};


template<class T>
struct cache_value {
	T value{};
	T previous{};

	cache_value() = default;
	explicit cache_value(const T val, const T prev) noexcept : value{ val }, previous{ prev } {}
	explicit cache_value(const T val) noexcept : value{ val }, previous{ val } {}

	bool changed() const noexcept { return value != previous; }
};


} // namespace golxzn::graphics::mods
