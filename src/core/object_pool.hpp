#pragma once

#include "gkit/core/object.hpp"
#include "gkit/core/object_id.hpp"
#include "gkit/core/templates/singleton.hpp"

#include <optional>
#include <unordered_map>

namespace gkit::core {
    /**
     * @brief Internal registry mapping ObjectId to live Object instances.
     *
     * Owns every Object allocated through UniqueObject: acquire() mints the
     * id and takes ownership, release() deletes the object and recycles the
     * id. This class is an implementation detail and is only reachable from
     * within the library sources.
     */
    class ObjectPool : public gkit::core::templates::Singleton<ObjectPool> {
        friend gkit::core::templates::Singleton<ObjectPool>;
        std::unordered_map<ObjectId, Object*> id_instance_map;

        ObjectPool()  = default;
        ~ObjectPool() = default;

    public:
        /**
         * @brief Mint a fresh ObjectId for @p obj and register it.
         * @param obj The heap-allocated object to take ownership of.
         * @return The assigned id, or std::nullopt if registration fails
         *         (e.g. allocation error). On failure the caller keeps
         *         ownership of @p obj and is responsible for freeing it.
         */
        auto acquire(Object* obj) noexcept -> std::optional<ObjectId>;

        /**
         * @brief Unregister and delete the object behind @p drop_id.
         * @param drop_id A valid id previously returned by acquire().
         */
        auto release(const ObjectId& drop_id) noexcept -> void;

        /**
         * @brief Look up the object behind @p id.
         * @param id A previously acquired id.
         * @return The object pointer, or nullptr if @p id is invalid or unknown.
         */
        auto deref_from(const ObjectId& id) noexcept -> Object*;
    };
} // namespace gkit::core
