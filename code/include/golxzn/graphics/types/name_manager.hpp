#pragma once

#include <set>
#include <string>
#include <optional>

#include "golxzn/core/types.hpp"

namespace golxzn::graphics::types {

class name_manager {
public:
	static constexpr std::string_view unnamed{ "_unnamed" };
	static constexpr std::string_view invalid_name{ "_invalid_name" };

	GOLXZN_STATIC_CLASS(name_manager);

	static std::string create_name(const std::string &name) noexcept;
	static void free_name(const std::string &name) noexcept;
	static bool contains(const std::string &name) noexcept;
	static bool is_free_name(const std::string &name) noexcept;
	static std::optional<core::u32> get_free_id_for(const std::string &name) noexcept;
	static std::optional<core::u32> extract_id(const std::string &name) noexcept;

private:
	static core::umap<std::string, std::set<core::u32>> mGivenNames;
	static std::string make(const std::string &name, const core::u32 id) noexcept;
};

class named {
public:
	explicit named(const std::string &name) noexcept;
	virtual ~named();

	named(const named &other);
	named(named &&other) noexcept = default;

	named &operator=(const named &other);
	named &operator=(named &&other) noexcept = default;

	void copy_from(const named &other) noexcept;
	void move_from(named &&other) noexcept;

	void rename(const std::string &name) noexcept;
	const std::string &full_name() const noexcept;
	std::string name() const noexcept;

private:
	std::string mName;
};

} // namespace golxzn::graphics::types
