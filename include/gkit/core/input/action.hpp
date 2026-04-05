#pragma once

#include "gkit/core/input/keys.hpp"
#include <variant>

namespace gkit {
    class Input;
}

namespace gkit::input {
    class Action {
        friend class gkit::Input;

    public:
        enum class Type {
            Key,
            MouseButton,
            GamepadButton,
        };

    using InputChord = std::variant<KeyChord>;

    public:
        Action() = default;
        ~Action() = default;

    public:
        auto get_type() const -> Type { return type; }
        auto set_action(const InputChord& action) -> void {
            std::visit([this](const auto& action) {
                using ChordT = std::decay_t<decltype(action)>;
                if constexpr (std::is_same_v<KeyChord, ChordT>) {
                    this->type = Type::Key;
                    this->chord = action;
                }
            }, action);
        }

    private:
        Type type;
        InputChord chord;
    }; // class Action
} // namespace gkit::input