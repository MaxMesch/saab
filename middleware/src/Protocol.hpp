#pragma once

#include <cstdint>
#include <vector>

#include "SaabObject.hpp"

namespace saab {

constexpr uint32_t kPreamble = 0xFE00;

// Preamble, count, then id/x/y/type/color per object. Native byte order.
std::vector<uint8_t> encode_report(const std::vector<ObjectState>& objects);

} // namespace saab
