#pragma once

#include <any>
#include <optional>
#include <golxzn/core/types.hpp>

namespace golxzn::graphics::ctrl {

class object {
public:
	GOLXZN_DEFAULT_CLASS(object);
	using ref = core::sptr<object>;

	static constexpr core::u32 invalid_id{ 0 };

	explicit object(const core::u32 id) noexcept;

	[[nodiscard]] core::u32 id() const noexcept;
	[[nodiscard]] bool valid() const noexcept;

	template<class T>
	std::optional<T> get(const std::string &key) const {
		if (auto found{ mProperties.find(key) }; found != std::end(mProperties)) {
			auto any{ found->second };
			if (auto value_ptr{ std::any_cast<T>(&any) }; value_ptr != nullptr) {
				return std::make_optional(*value_ptr);
			}
		}
		return std::nullopt;
	}

	template<class T>
	void set(const std::string &key, const T &value) {
		mProperties.emplace(key, std::make_any(value));
	}

	template<class T>
	void set(const std::string &key, T &&value) {
		mProperties[key] = std::move(value);
	}

private:
	core::u32 mId{ invalid_id };
	core::umap<std::string, std::any> mProperties;
};

} // namespace golxzn::graphics::ctrl
