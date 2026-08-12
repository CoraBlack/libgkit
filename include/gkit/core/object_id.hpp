#pragma once

#include "gkit/core/object.hpp"
#include "gkit/core/templates/gen_id.hpp"

#include <functional>

namespace gkit::core {
    class ObjectIdTag;

    /**
     * @brief Object ID backed by GenId, giving each object a unique id with a
     *        monotonic generation to guard against stale references.
     */
    class ObjectId final : public templates::GenId<ObjectIdTag> {
    public:
        ObjectId() noexcept = default;

        explicit ObjectId(const templates::GenId<ObjectIdTag>& base) noexcept : templates::GenId<ObjectIdTag>(base) {}

        auto deref() const noexcept -> Object*;

        template<IsObject T>
        auto deref_as() const noexcept -> T*;
    };

    template<IsObject T>
    auto ObjectId::deref_as() const noexcept -> T* {
        return dynamic_cast<T*>(this->deref());
    }
} // namespace gkit::core



template<>
struct std::hash<gkit::core::ObjectId> {
    auto operator()(const gkit::core::ObjectId& obj_id) const -> std::size_t {
        return std::hash<gkit::core::templates::GenId<gkit::core::ObjectIdTag>>{}(obj_id);
    }
};
