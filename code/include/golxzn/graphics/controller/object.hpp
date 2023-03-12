#pragma once

#include <any>
#include <optional>
#include <golxzn/core/types.hpp>

namespace golxzn::graphics::ctrl {

class object {
public:
	using ref = core::sptr<object>;
	using deleter = std::function<void(object &)>;

	static constexpr auto default_deleter{ [](object&) {} };
	static constexpr core::u32 invalid_id{ 0 };

	static ref make(const core::u32 id = invalid_id, deleter &&deleter = default_deleter);

	object() = default;
	explicit object(const core::u32 id, deleter &&deleter = default_deleter) noexcept;
	~object();

	void set_deleter(deleter &&deleter) noexcept;

	[[nodiscard]] core::u32 id() const noexcept;
	[[nodiscard]] bool valid() const noexcept;

	template<class T>
	std::optional<T> get_property(const std::string &key) const {
		if (auto found{ mProperties.find(key) }; found != std::end(mProperties)) {
			auto any{ found->second };
			if (auto value_ptr{ std::any_cast<T>(&any) }; value_ptr != nullptr) {
				return std::make_optional(*value_ptr);
			}
		}
		return std::nullopt;
	}

	template<class T>
	void set_property(const std::string &key, const T &value) {
		mProperties.emplace(key, std::make_any<T>(value));
	}

	template<class T>
	void set_property(const std::string &key, T &&value) {
		mProperties[key] = std::move(value);
	}

private:
	core::u32 mId{ invalid_id };
	deleter mDeleter{ default_deleter };
	core::umap<std::string, std::any> mProperties;
};

} // namespace golxzn::graphics::ctrl
