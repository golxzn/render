#pragma once

#include <any>
#include <optional>
#include <golxzn/core/aliases.hpp>

namespace golxzn::graphics::types {

class object {
public:
	using ref = core::sptr<object>;
	using deleter = std::function<void(object &)>;
	using id_t = core::u32;

	static constexpr auto default_deleter{ [](object&) {} };
	static constexpr id_t invalid_id{ 0 };

	static ref make(const id_t id = invalid_id, deleter &&deleter = default_deleter);

	object() = default;
	explicit object(const id_t id, deleter &&deleter = default_deleter) noexcept;
	~object();

	void set_deleter(deleter &&deleter) noexcept;

	[[nodiscard]] id_t id() const noexcept;
	[[nodiscard]] bool valid() const noexcept;

	template<class T>
	bool is_type_a(const std::string_view key) const {
		if (auto found{ mProperties.find(std::string{ key }) }; found != std::end(mProperties)) {
			auto any{ found->second };
			return any.type() == typeid(T);
		}
		return false;
	}

	template<class T>
	std::optional<T> get_property(const std::string_view key) const {
		if (auto found{ mProperties.find(std::string{ key }) }; found != std::end(mProperties)) {
			auto any{ found->second };
			if (auto value_ptr{ std::any_cast<T>(&any) }; value_ptr != nullptr) {
				return std::make_optional(*value_ptr);
			}
		}
		return std::nullopt;
	}

	template<class T>
	void set_property(const std::string_view key, const T &value) {
		mProperties.insert_or_assign(std::string{ key }, std::make_any<T>(value));
	}

	template<class T>
	void set_property(const std::string_view key, T &&value) {
		mProperties.insert_or_assign(std::string{ key }, std::make_any<T>(std::move(value)));
	}

private:
	id_t mId{ invalid_id };
	deleter mDeleter{ default_deleter };
	core::umap<std::string, std::any> mProperties;
};

} // namespace golxzn::graphics::types
