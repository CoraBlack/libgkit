#include "gkit/core/unique_object.hpp"

#include "gkit/core/object_id.hpp"
#include "gkit/core/reflect/registry.hpp"
#include "object_pool.hpp"

#include <string>
#include <utility>

namespace gkit::core {
    UniqueObject::UniqueObject(std::string&& class_name) noexcept : id(ObjectIdAllocator::instance().new_one()) {
        (void)class_name;
        auto& db = reflect::ClassDB::instance();
        // TODO: cora - wait for factory method from feature/reflection
    }

    UniqueObject::UniqueObject(UniqueObject&& other) noexcept : id(std::move(other.id)) {
        this->obj = other.obj;
        other.obj = nullptr;
    }

    UniqueObject::~UniqueObject() noexcept {
        auto& obj_pool = ObjectPool::instance();
        obj_pool.release(this->id);
    }

    auto UniqueObject::operator=(UniqueObject&& other) noexcept -> UniqueObject& {
        if (this != &other) {
            auto& obj_pool = ObjectPool::instance();
            obj_pool.release(this->id);
            this->id  = std::move(other.id);
            this->obj = other.obj;
            other.obj = nullptr;
        }
        return *this;
    }
} // namespace gkit::core