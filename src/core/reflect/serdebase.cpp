#include "gkit/core/reflect/serdebase.hpp"

#include <stdexcept>

namespace gkit::core::reflect {

    auto SerdeBase::seralize(const ObjectId& id) -> std::string {
        if (!id.available()) {
            throw std::invalid_argument("id is not available");
        }

        SerdeStruct ss(id);
        if (!ss.available()) {
            return {};
        }

        return this->render(ss.root());
    }

    auto SerdeBase::seralize(const std::string& key, const Value& v) -> std::string {
        return this->render(SerdeNode(key, v));
    }

    auto SerdeBase::render(const SerdeNode& node) -> std::string {
        auto res = std::string();

        if (!node.get_key().empty()) {
            auto key_wrap = this->key_wrapper(node.get_key());
            res += key_wrap.first + node.get_key() + key_wrap.second;
            res += this->kv_connect();
        }

        switch (node.get_type()) {
        case Type::Array:
        case Type::Map:
        case Type::Object: {
            auto wrap            = this->container_wrapper(node.get_type(), node.get_key());
            const auto& children = node.get_children();
            res += wrap.first;

            for (auto it = children.cbegin(); it != children.cend(); ++it) {
                res += this->render(**it);
                if (it + 1 != children.cend()) {
                    res += this->ele_gap(node.get_type());
                }
            }

            res += wrap.second;
            break;
        }

        default: {
            res += this->value_wrapper(node.get_val(), node.get_type());
            break;
        }
        }

        return res;
    }

    auto SerdeBase::container_wrapper(Type type, const std::string& name) -> WrapperPair {
        switch (type) {
        case Type::Array: {
            return this->array_wrapper(name);
        }
        case Type::Map: {
            return this->map_wrapper(name);
        }
        default: {
            return this->object_wrapper(name);
        }
        }
    }

    auto SerdeBase::ele_gap(Type type) -> std::string {
        switch (type) {
        case Type::Array: {
            return this->array_ele_gap();
        }
        case Type::Map: {
            return this->map_ele_gap();
        }
        default: {
            return this->object_ele_gap();
        }
        }
    }

} // namespace gkit::core::reflect
