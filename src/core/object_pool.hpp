#pragma once

#include "gkit/core/object.hpp"
#include "gkit/core/object_id.hpp"
#include "gkit/core/templates/singleton.hpp"

#include <optional>
#include <unordered_map>

namespace gkit::core {
    class ObjectPool : public gkit::core::templates::Singleton<ObjectPool> {
        friend gkit::core::templates::Singleton<ObjectPool>;
        std::unordered_map<ObjectId, Object*> id_instance_map;

        ObjectPool()  = default;
        ~ObjectPool() = default;

    public:
        auto acquire(Object* obj) noexcept -> std::optional<ObjectId>;
        auto release(const ObjectId& drop_id) noexcept -> void;
        auto deref_from(const ObjectId& id) noexcept -> Object*;
    };
} // namespace gkit::core