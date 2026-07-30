#include "gkit/core/unique_object.hpp"

#include "gkit/core/object_id.hpp"
#include "gkit/core/reflect/registry.hpp"

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
        if (this->obj != nullptr) {
            delete this->obj;
        }

        if (this->id.available()) {
            auto& id_alloc = ObjectIdAllocator::instance();
            id_alloc.drop(this->id);
        }
    }
} // namespace gkit::core