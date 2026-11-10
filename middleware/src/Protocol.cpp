#include "Protocol.hpp"

#include <array>

namespace saab {

namespace {

template <typename T>
void append_raw(std::vector<uint8_t>& out, const T& value) {
    const auto* bytes = reinterpret_cast<const uint8_t*>(&value);
    out.insert(out.end(), bytes, bytes + sizeof(T));
}

constexpr std::array<uint8_t, 3> kRedBytes{0x5B, 0x31, 0x6D};
constexpr std::array<uint8_t, 3> kYellowBytes{0x5B, 0x33, 0x6D};
constexpr std::array<uint8_t, 3> kBlueBytes{0x5B, 0x34, 0x6D};

void append_color(std::vector<uint8_t>& out, Color color) {
    switch (color) {
    case Color::Red:
        out.insert(out.end(), kRedBytes.begin(), kRedBytes.end());
        break;
    case Color::Yellow:
        out.insert(out.end(), kYellowBytes.begin(), kYellowBytes.end());
        break;
    case Color::Blue:
        out.insert(out.end(), kBlueBytes.begin(), kBlueBytes.end());
        break;
    }
}

} // namespace

std::vector<uint8_t> encode_report(const std::vector<ObjectState>& objects) {
    std::vector<uint8_t> out;
    out.reserve(sizeof(uint32_t) * 2 + objects.size() * (8 + 4 + 4 + 1 + 3));

    append_raw(out, kPreamble);
    append_raw(out, static_cast<uint32_t>(objects.size()));

    for (const auto& obj : objects) {
        append_raw(out, obj.id);
        append_raw(out, obj.x);
        append_raw(out, obj.y);
        append_raw(out, obj.type);
        append_color(out, color_for(obj));
    }

    return out;
}

} // namespace saab
