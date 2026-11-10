#pragma once

#include <cstdint>
#include <optional>

namespace saab {

// One object's latest known state, as received from the SAAB server.
struct ObjectState {
    int64_t id = 0;
    int32_t x = 0;
    int32_t y = 0;
    uint8_t type = 0;
};

enum class Color : uint8_t { Red, Yellow, Blue };

// Category 1: types 1 and 2. Category 2: type 3.
enum class Category : uint8_t { One, Two };

// Distances are measured from here.
constexpr int32_t kReferenceX = 150;
constexpr int32_t kReferenceY = 150;

// Category of a type, or nullopt if the type is unknown.
std::optional<Category> category_of(uint8_t type);

// Throws std::invalid_argument if no color rule matches obj.type.
Color color_for(const ObjectState& obj);

} // namespace saab
