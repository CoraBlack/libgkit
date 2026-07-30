#pragma once

#include "gkit/core/object.hpp"
#include "object_id.hpp"

namespace gkit::core {
    class UniqueObject final {
        Object* obj = nullptr;
        ObjectId id;

    public:
        template<IsObject T>
        UniqueObject() noexcept;
        explicit UniqueObject(std::string&& class_name) noexcept;
        ~UniqueObject() noexcept;
        UniqueObject(const UniqueObject&) noexcept = delete;
        UniqueObject(UniqueObject&& other) noexcept;

        auto operator=(const UniqueObject& other) -> ObjectId;
    };

    template<IsObject T>
    UniqueObject::UniqueObject() noexcept {
        try {
            this->obj = new T();
            this->id  = ObjectIdAllocator::instance().new_one();
        } catch (...) {
            this->obj = nullptr;
        }
    }
} // namespace gkit::core