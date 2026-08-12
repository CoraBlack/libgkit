#pragma once

#include "gkit/core/object.hpp"
#include "gkit/core/object_id.hpp"
#include "gkit/core/templates/singleton.hpp"

#include <optional>
#include <unordered_map>
#include <utility>

namespace gkit::core {
    class ObjectPool : public gkit::core::templates::Singleton<ObjectPool> {
        friend gkit::core::templates::Singleton<ObjectPool>;
        std::unordered_map<ObjectId, Object*> id_instance_map;

        ObjectPool()  = default;
        ~ObjectPool() = default;

    public:
        template<IsObject T, class... Args>
        auto create(Args&&...) noexcept -> std::optional<std::pair<ObjectId, Object*>>;
        auto release(const ObjectId& drop_id) noexcept -> void;
        auto deref_from(const ObjectId& id) noexcept -> Object*;
    };

    template<IsObject T, class... Args>
    auto ObjectPool::create(Args&&... args) noexcept -> std::optional<std::pair<ObjectId, Object*>> {
        try {
            auto* obj_ptr = new T(std::forward<Args>(args)...);
            auto obj_id   = ObjectId(ObjectId::IdAllocator::instance().new_one());
            this->id_instance_map.emplace(obj_id, obj_ptr);
            return std::make_pair(obj_id, obj_ptr);
        } catch (...) {
            return std::nullopt;
        }
    }
} // namespace gkit::core