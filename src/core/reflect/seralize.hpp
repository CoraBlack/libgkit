#pragma once

#include "gkit/core/object_id.hpp"
#include "gkit/core/value.hpp"

#include <memory>
#include <string>
#include <vector>

namespace gkit::core::reflect {
    class SerdeNode;

    class SerdeStruct final {
        bool available_flag = false;
        std::unique_ptr<SerdeNode> serde_root{};

    public:
        explicit SerdeStruct(const Value v) noexcept;
        explicit SerdeStruct(const ObjectId v) noexcept;
        ~SerdeStruct() = default;

        [[nodiscard]] inline auto available() -> bool { return this->available_flag; }
        [[nodiscard]] inline auto root() const -> const SerdeNode& { return *this->serde_root; }
    }; // class SerdeStruct

    class SerdeNode {
        friend SerdeStruct;
        std::string key; // only available when type is Map or parent is Object
        std::string val; // only available when type is basic data value
        Type type = Type::Null;

        // if type is basic data or array type, it will no key value.
        // if type is map, SerdeNode is the kv pair.
        // if type is Object, SerdeNode children are as the field(name, value) list.
        std::vector<std::unique_ptr<SerdeNode>> children;

    public:
        explicit SerdeNode(const std::string& k, const Value& v) noexcept;
        explicit SerdeNode(const std::string& k, const std::string& v, Type t) noexcept;
        SerdeNode(const SerdeNode&) = delete;
        SerdeNode(SerdeNode&&)      = default;
        ~SerdeNode() noexcept       = default;

        auto add_child(SerdeNode&& child) -> void;

        [[nodiscard]] inline auto get_key() const -> const std::string& { return this->key; }
        [[nodiscard]] inline auto get_val() const -> const std::string& { return this->val; }
        [[nodiscard]] inline auto get_type() const -> Type { return this->type; }
        [[nodiscard]] inline auto get_children() const -> const std::vector<std::unique_ptr<SerdeNode>>& {
            return this->children;
        }
    }; // class SerdeNode
} // namespace gkit::core::reflect