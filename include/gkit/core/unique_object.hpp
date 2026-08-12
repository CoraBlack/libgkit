#pragma once

#include "gkit/core/object.hpp"
#include "object_id.hpp"

#include <optional>
#include <utility>

namespace gkit::core {
    class UniqueObject final {
        Object* obj = nullptr;
        ObjectId id;

        UniqueObject() = default;

    public:
        ~UniqueObject() noexcept;
        UniqueObject(const UniqueObject&) noexcept = delete;
        UniqueObject(UniqueObject&& other) noexcept;
        auto operator=(const UniqueObject&) noexcept = delete;
        auto operator=(UniqueObject&& other) noexcept -> UniqueObject&;

        [[nodiscard]] auto get() -> Object* { return this->obj; }
        [[nodiscard]] auto get() const -> const Object* { return this->obj; }
        [[nodiscard]] inline auto get_id() const -> const ObjectId& { return this->id; }

        template<IsObject T, class... Args>
        static auto create(Args&&... args) noexcept -> UniqueObject;
        static auto create_with_classname(const std::string& class_name) noexcept -> std::optional<UniqueObject>;

    private:
        static auto from_raw_ptr(Object* obj) noexcept -> UniqueObject;
    };

    template<IsObject T, class... Args>
    auto UniqueObject::create(Args&&... args) noexcept -> UniqueObject {
        try {
            return from_raw_ptr(new T(std::forward<Args>(args)...));
        } catch (...) {
            return UniqueObject();
        }
    }
} // namespace gkit::core