#include "gkit/core/object_id.hpp"

#include <mutex>

namespace gkit::core {

    /**
     * ObjectId
     */
    ObjectId::ObjectId() noexcept {}
    ObjectId::ObjectId(ObjectId&& other) noexcept : id(other.id), version(other.version) {
        other.id      = 0;
        other.version = 0;
    }

    /**
     * ObjectIdAllocator
     */
    ObjectIdAllocator::ObjectIdAllocator() noexcept {
        // init value, every id is alloc from 1
        // id equal zero means not alloc or invalid
        this->id_pool.push(1);
    }

    auto ObjectIdAllocator::new_one() noexcept -> ObjectId {
        auto obj_id = ObjectId();
        {
            std::unique_lock alloc_locker(this->id_alloc_lock);
            if (this->id_pool.size() <= 1) {
                // Final element in id_pool is not alloc before be called top().
                // So the version of final element always is one.
                obj_id.id      = id_pool.top()++;
                obj_id.version = 1;
            } else {
                // version of recycle id is not always one
                // and is recorded to version map when the id is dropped.
                obj_id.id = id_pool.top();
                id_pool.pop();
                obj_id.version = ++this->id_version[obj_id.id];
            }
        }
        return obj_id;
    }

    auto ObjectIdAllocator::drop(const ObjectId& obj_id) noexcept -> void {
        auto old_id      = obj_id.id;
        auto old_version = obj_id.version;

        {
            std::unique_lock alloc_locker(this->id_alloc_lock);
            this->id_pool.push(old_id);
            auto version_it = this->id_version.find(old_id);

            // no record version ever
            if (version_it == this->id_version.end()) {
                this->id_version[old_id] = 1;
            }
        }
    }
} // namespace gkit::core