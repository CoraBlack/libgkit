#pragma once

#include "gkit/scene/2d/transform2d.hpp"
#include "gkit/core/scene/object.hpp"

namespace gkit::scene {
    class Object2D : public gkit::core::scene::Object {
    public:
        Object2D() = default;
        virtual ~Object2D() = default;

    public:
        /***********************************
         * Transform2D
         **********************************/
        inline auto get_transform() -> Transform2D& { return transform; }
        inline auto get_transform() const -> const Transform2D& { return transform; }
        inline auto get_position() const -> math::Vector2 { return transform.get_position(); }
        inline auto set_position(const math::Vector2& position) -> void { transform.set_position(position); }
        inline auto get_rotation() const -> float { return transform.get_rotation(); }
        inline auto set_rotation(const float ROTATION) -> void { transform.set_rotation(ROTATION); }
        inline auto get_scale() const -> math::Vector2 { return transform.get_scale(); }
        inline auto set_scale(const math::Vector2& scale) -> void { transform.set_scale(scale); }

    private:
        Transform2D transform;
    };
} // namespace gkit::scene