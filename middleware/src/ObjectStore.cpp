#include "ObjectStore.hpp"

namespace saab {

void ObjectStore::upsert(const ObjectState& obj) { objects_[obj.id] = obj; }

std::vector<ObjectState> ObjectStore::snapshot() const {
    std::vector<ObjectState> result;
    result.reserve(objects_.size());
    for (const auto& [id, obj] : objects_) {
        result.push_back(obj);
    }
    return result;
}

} // namespace saab
