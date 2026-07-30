#pragma once

#include "core/object_pool.hpp"
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

        inline auto get_id() const -> const ObjectId& { return this->id; }
    };

    template<IsObject T>
    UniqueObject::UniqueObject() noexcept {
        auto& obj_pool = ObjectPool::instance();
        auto obj_opt   = obj_pool.create<T>();
        if (obj_opt.has_value()) {
            this->id  = obj_opt->first;
            this->obj = obj_opt->second;
        }
    }
} // namespace gkit::core