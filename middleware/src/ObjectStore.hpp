#pragma once

#include <unordered_map>
#include <vector>

#include "SaabObject.hpp"

namespace saab {

// Latest state per object id.
class ObjectStore {
public:
    void upsert(const ObjectState& obj);
    std::vector<ObjectState> snapshot() const;

private:
    std::unordered_map<int64_t, ObjectState> objects_;
};

} // namespace saab
