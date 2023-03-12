#include <spdlog/spdlog.h>

#include "golxzn/graphics/types/name_manager.hpp"


namespace golxzn::graphics::types {

core::umap<std::string, std::set<core::u32>> name_manager::mGivenNames {};

std::string name_manager::create_name(const std::string &given_name) noexcept {
	if (given_name.empty()) {
		return create_name(std::string{ unnamed });
	}

	if (!contains(given_name)) {
		mGivenNames.emplace(given_name, std::set<core::u32>{ 0 });
		return make(given_name, 0);
	}

	const auto id{ get_free_id_for(given_name) };
	if (!id.has_value()) {
		spdlog::error("name_manager::create_name: Failed to get free id for given name '{}'", given_name);
		return create_name(std::string{ invalid_name });
	}
	if (id.value() == 0) {
		return make(given_name, id.value());
	}
	mGivenNames[given_name].emplace(id.value());
	return make(given_name, id.value());
}

void name_manager::free_name(const std::string &name) noexcept {
	if (name.empty()) {
		return;
	}

	const auto id{ extract_id(name) };
	if (!id.has_value()) {
		return;
	}

	if (const auto found{ mGivenNames.find(name) }; found != std::end(mGivenNames)) {
		auto &ids{ found->second };
		ids.erase(id.value());
		if (ids.empty()) {
			mGivenNames.erase(found);
		}
	}
}

bool name_manager::contains(const std::string &name) noexcept {
	return mGivenNames.find(name) != std::end(mGivenNames);
}

bool name_manager::is_free_name(const std::string &name) noexcept {
	const auto id{ extract_id(name) };
	if (!id.has_value()) {
		return false;
	}

	const auto found{ mGivenNames.find(name) };
	if (found == std::end(mGivenNames)) {
		return true;
	}
	const auto &ids{ found->second };
	return ids.find(id.value()) == std::end(ids);
}

std::optional<core::u32> name_manager::get_free_id_for(const std::string &name) noexcept {
	if (name.empty()) {
		return std::nullopt;
	}

	const auto found{ mGivenNames.find(name) };
	if (found == std::end(mGivenNames)) {
		return core::u32{ 0 };
	}
	const auto &ids{ found->second };

	core::u32 prev_id{};
	const auto is_insertion = std::any_of(std::begin(ids), std::end(ids), [&prev_id](const auto &id) {
		return id != prev_id++;
	});
	return is_insertion ? prev_id - 1 : prev_id;
}

std::optional<core::u32> name_manager::extract_id(const std::string &name) noexcept {
	if (name.empty()) {
		return std::nullopt;
	}

	const auto found{ name.find_last_of('_') };
	if (found == std::string::npos) {
		return core::u32{ 0 };
	}

	const auto str_id{ name.substr(found + 1) };
	const bool digit{ std::all_of(std::begin(str_id), std::end(str_id), [](const auto &c) {
		return std::isdigit(c);
	}) };
	if (digit) {
		return std::strtoul(str_id.data(), nullptr, 10);
	}
	return std::nullopt;
}

std::string name_manager::make(const std::string &name, const core::u32 id) noexcept {
	return name + '_' + std::to_string(id);
}

//======================================== named ========================================//

named::named(const std::string &name) noexcept : mName{ name_manager::create_name(name) } {}
named::~named() {
	name_manager::free_name(mName);
}

named::named(const named &other)
	: mName{ name_manager::create_name(other.name()) } {}

named &named::operator=(const named &other) {
	if (this == &other) {
		return *this;
	}

	copy_from(other);
	return *this;
}

void named::copy_from(const named &other) noexcept {
	rename(other.name());
}
void named::move_from(named &&other) noexcept {
	mName = std::move(other.mName);
}

void named::rename(const std::string &name) noexcept {
	name_manager::free_name(mName);
	mName = name_manager::create_name(name);
}

const std::string &named::full_name() const noexcept {
	return mName;
}
std::string named::name() const noexcept {
	if (const auto found{ mName.find_last_of('_') }; found == std::string::npos) {
		return mName.substr(0, found);
	}
	return mName;
}

} // namespace golxzn::graphics::types
