#pragma once

#include "gkit/core/object_id.hpp"
#include "gkit/core/value.hpp"

#include <memory>
#include <string>
#include <vector>

namespace gkit::core::reflect {
    class SerdeNode {
        std::string key;
        Type type = Type::Null;
        Value value; // leaf payload; Null for container nodes
        std::vector<std::unique_ptr<SerdeNode>> children;

    public:
        explicit SerdeNode(const std::string& k, const Value& v) noexcept;
        explicit SerdeNode(const std::string& k, Type t) noexcept;
        SerdeNode(const SerdeNode&) = delete;
        SerdeNode(SerdeNode&&)      = default;
        ~SerdeNode() noexcept       = default;

        auto add_child(SerdeNode&& child) -> void;

        [[nodiscard]] inline auto get_key() const -> const std::string& { return this->key; }
        [[nodiscard]] inline auto get_type() const -> Type { return this->type; }
        [[nodiscard]] inline auto get_value() const -> const Value& { return this->value; }
        [[nodiscard]] inline auto get_children() const -> const std::vector<std::unique_ptr<SerdeNode>>& {
            return this->children;
        }
    };

    class SerdeStruct final {
        bool available_flag = false;
        std::unique_ptr<SerdeNode> serde_root{};

    public:
        explicit SerdeStruct(Value v) noexcept;
        explicit SerdeStruct(const ObjectId v) noexcept;
        ~SerdeStruct() = default;

        [[nodiscard]] inline auto available() -> bool { return this->available_flag; }
        [[nodiscard]] inline auto root() const -> const SerdeNode& { return *this->serde_root; }
    };
} // namespace gkit::core::reflect
