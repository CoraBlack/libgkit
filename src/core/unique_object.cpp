#include "gkit/core/unique_object.hpp"

#include "gkit/core/object_id.hpp"
#include "gkit/core/reflect/registry.hpp"
#include "object_pool.hpp"

#include <string>
#include <utility>

namespace gkit::core {
    auto UniqueObject::create_with_classname(const std::string& class_name) noexcept -> std::optional<UniqueObject> {
        return reflect::ClassDB::instance().create(class_name);
    }

    auto UniqueObject::from_raw_ptr(Object* obj) noexcept -> UniqueObject {
        UniqueObject uobj;
        if (obj == nullptr) return uobj;

        auto& pool  = ObjectPool::instance();
        auto id_opt = pool.acquire(obj);
        if (id_opt.has_value()) {
            uobj.id  = *id_opt;
            uobj.obj = obj;
        } else {
            // Registration failed and the pool never took ownership;
            // free the raw pointer to avoid a leak.
            delete obj;
        }

        return uobj;
    }

    UniqueObject::UniqueObject(UniqueObject&& other) noexcept : id(other.id) {
        this->obj = other.obj;
        other.obj = nullptr;
        // Reset the source id so the moved-from handle releases nothing.
        other.id = ObjectId();
    }

    UniqueObject::~UniqueObject() noexcept {
        auto& obj_pool = ObjectPool::instance();
        obj_pool.release(this->id);
    }

    auto UniqueObject::operator=(UniqueObject&& other) noexcept -> UniqueObject& {
        if (this != &other) {
            auto& obj_pool = ObjectPool::instance();
            obj_pool.release(this->id);
            this->id  = other.id;
            this->obj = other.obj;
            other.obj = nullptr;
            other.id  = ObjectId();
        }
        return *this;
    }
} // namespace gkit::core