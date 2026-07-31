#pragma once

#include "gkit/core/templates/singleton.hpp"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <mutex>
#include <stack>
#include <unordered_map>

namespace gkit::core {
    class ObjectIdAllocator;

    class ObjectId final {
        friend ObjectIdAllocator;
        friend std::hash<gkit::core::ObjectId>;
        // defalut is invalid value
        uint32_t id      = 0u;
        uint32_t version = 0u;

    public:
        ObjectId() noexcept;

    public:
        ObjectId(const ObjectId& other) noexcept = default;
        ObjectId(ObjectId&& other) noexcept;
        auto operator=(const ObjectId& other) -> ObjectId& = default;
        auto operator=(ObjectId&& other) -> ObjectId&      = default;

        auto operator==(const ObjectId& other) const -> bool {
            return this->id == other.id && this->version == other.version;
        }

        /**
         * @brief check ObjectId is invalid
         * @return bool - Both id and version are not zero.
         */
        inline auto available() const -> bool { return this->id != 0 && this->version != 0; }
    }; // class ObjectId

    class ObjectIdAllocator final : public gkit::core::templates::Singleton<ObjectIdAllocator> {
        friend gkit::core::templates::Singleton<ObjectIdAllocator>;
        std::mutex id_alloc_lock{};
        std::stack<uint32_t> id_pool{};
        std::unordered_map<uint32_t, uint32_t> id_version{};

        ObjectIdAllocator() noexcept;
        ~ObjectIdAllocator() = default;

    public:
        /**
         * @brief alloc a new ObjectId instance
         * @return ObjectId
         */
        auto new_one() noexcept -> ObjectId;

        /**
         * @brief drop an ObjectId and recycle its id
         * @param obj_id - which ObjectId is needed to be dropped
         */
        auto drop(const ObjectId& obj_id) noexcept -> void;
    }; // class ObjectIdAllocator
} // namespace gkit::core

template<>
struct std::hash<gkit::core::ObjectId> {
    auto operator()(const gkit::core::ObjectId& objid) const -> std::size_t {
        auto uint_hash = std::hash<uint32_t>();
        return uint_hash(objid.id) ^ uint_hash(objid.version);
    }
};