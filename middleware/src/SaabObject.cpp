#include "SaabObject.hpp"

#include <cmath>
#include <stdexcept>
#include <string>

namespace saab {

namespace {

double distance_from_reference(int32_t x, int32_t y) {
    const double dx = static_cast<double>(x) - kReferenceX;
    const double dy = static_cast<double>(y) - kReferenceY;
    return std::sqrt(dx * dx + dy * dy);
}

} // namespace

// type 1 and 2 is category 1, type 3 is category 2. In future could make a better mapping if function expands.
std::optional<Category> category_of(uint8_t type) {
    switch (type) {
    case 1:
    case 2:
        return Category::One;
    case 3:
        return Category::Two;
    default:
        return std::nullopt;
    }
}

Color color_for(const ObjectState& obj) {
    const double dist = distance_from_reference(obj.x, obj.y);
    const auto category = category_of(obj.type);

    if (category == Category::One) {
        // Category 1: thresholds are per type.
        switch (obj.type) {
        case 1: // Blue, yellow under 75, red under 50.
            if (dist < 50.0) return Color::Red;
            if (dist < 75.0) return Color::Yellow;
            return Color::Blue;
        case 2: // Blue, yellow under 50 (never turns red).
            return dist < 50.0 ? Color::Yellow : Color::Blue;
        }
    } else if (category == Category::Two) {
        // Category 2: yellow, red under 100. Applies to every type in the category.
        return dist < 100.0 ? Color::Red : Color::Yellow;
    }

    throw std::invalid_argument("no color rule for type " + std::to_string(obj.type));
}

} // namespace saab
