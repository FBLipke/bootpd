#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include <map>

// Basic types for plugin API (minimal, no platform dependencies)
namespace bootp::plugins::api
{
	// Integer types
	using int8 = int8_t;
	using int16 = int16_t;
	using int32 = int32_t;
	using int64 = int64_t;
	using uint8 = uint8_t;
	using uint16 = uint16_t;
	using uint32 = uint32_t;
	using uint64 = uint64_t;

	// String type (platform-independent)
	using string = std::string;

	// Size type
	using size = size_t;

	// Boolean
	using boolean = bool;

	// Shared pointer
	template<typename T>
	using shared_ptr = std::shared_ptr<T>;

	// Vector and map
	template<typename T>
	using vector = std::vector<T>;

	template<typename K, typename V>
	using map = std::map<K, V>;
}
