#include "core/reflect/seralize.hpp"

#include "core/object_pool.hpp"
#include "gkit/core/reflect/registry.hpp"
#include "gkit/core/value.hpp"

#include <memory>
#include <string>
#include <utility>

namespace gkit::core::reflect {
    /* static auto to_json_string(const Value& v) noexcept -> std::string {

    } */

    /**
     * SerdeStruct
     */
    SerdeStruct::SerdeStruct(const ObjectId v) noexcept {
        if (!v.available()) return;
        this->serde_root = std::make_unique<SerdeNode>("", "", Type::Object);

        // TODO(impl) cora -replace deref_from by ObjectId::deref or dereference operator
        auto* v_ptr = ObjectPool::instance().deref_from(v);
        if (v_ptr == nullptr) return;

        auto& db         = ClassDB::instance();
        auto* class_info = db.find(v_ptr->class_name());
        if (class_info == nullptr) return;

        class_info->for_each_field(
            [this, &class_info, v_ptr](const FieldDesc& field, const auto&, const auto*) -> void {
                auto val_opt = class_info->get_field(v_ptr, field.name);
                if (!val_opt.has_value()) return;

                this->serde_root->add_child(SerdeNode(field.name, val_opt.value()));
            });

        this->available_flag = true;
    }

    /**
     * @brief Encode a leaf node value as a plain (Json-ish) string.
     * Format-neutral storage means String values carry no quotes.
     */
    static auto format_leaf(const SerdeNode& node) -> std::string {
        switch (node.get_type()) {
        case Type::String: {
            return "\"" + node.get_val() + "\"";
        }
        case Type::Null: {
            return "null";
        }
        default: {
            return node.get_val();
        }
        }
    }

    auto SerdeStruct::to_string() const -> std::string {
        auto res = std::string();

        auto gap         = ' ';
        auto begin_end   = std::pair<char, char>();
        const auto& root = this->serde_root;
        if (root->type == Type::Array) {
            gap              = ',';
            begin_end.first  = '[';
            begin_end.second = ']';
        } else if (root->type == Type::Map || root->type == Type::Object) {
            gap              = ',';
            begin_end.first  = '{';
            begin_end.second = '}';
        }

        res.push_back(begin_end.first);
        for (auto it = root->children.cbegin(); it != root->children.cend(); ++it) {
            const auto& node = *it;
            if (root->type == Type::Array) {
                res += format_leaf(*node);
            } else if (root->type == Type::Map || root->type == Type::Object) {
                res += "\"";
                res += node->key;
                res += "\":";
                res += format_leaf(*node);
            }

            if (it + 1 != root->children.cend()) {
                res.push_back(gap);
            }
        }

        res.push_back(begin_end.second);
        return res;
    }

    /**
     * SerdeNode
     */
    SerdeNode::SerdeNode(const std::string& k, const Value& v) noexcept {
        this->key  = k;
        this->type = v.type();
        if (this->type == Type::Object) {
            const auto* obj        = v.as_object_ptr();
            auto& db               = ClassDB::instance();
            const auto* class_info = db.find(obj->class_name());
            class_info->for_each_field(
                [this, &obj, &class_info](const FieldDesc& field, const auto&, const auto*) -> void {
                    auto val_opt = class_info->get_field(obj, field.name);
                    if (val_opt.has_value()) {
                        this->add_child(SerdeNode(field.name, val_opt.value()));
                    }
                });
        } else if (this->type == Type::Array) {
            const auto& arr = v.as_array();
            for (const auto& v : arr) {
                this->add_child(SerdeNode("", v));
            }
        } else if (this->type == Type::Map) {
            const auto& map = v.as_map();
            for (const auto& p : map) {
                this->add_child(SerdeNode(p.first, p.second));
            }
        } else {
            // Basic data type
            // test impl
            if (this->type == Type::Null) {
                this->val = "";
            } else if (this->type == Type::Bool) {
                this->val = v.as_bool() ? "true" : "false";
            } else if (this->type == Type::Number) {
                this->val = [this, &v]() -> std::string {
                    if (v.is_number_float()) {
                        return std::to_string(v.as_float());
                    } else {
                        return std::to_string(v.as_int64());
                    }
                }();
            } else if (this->type == Type::String) {
                this->val = v.as_string();
            }
        }
    }

    SerdeNode::SerdeNode(const std::string& k, const std::string& v, Type t) noexcept {
        this->type = t;
    }

    auto SerdeNode::add_child(SerdeNode&& child) -> void {
        this->children.emplace_back(std::make_unique<SerdeNode>(std::move(child)));
    }
} // namespace gkit::core::reflect
