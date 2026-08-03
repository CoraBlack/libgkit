#include "gkit/core/reflect/serdebase.hpp"

#include "core/reflect/seralize.hpp"

#include <stdexcept>
#include <string>

namespace gkit::core::reflect {

    namespace {

        auto render_node(const SerdeNode& node, SerdeBase& base) -> std::string {
            // ObjectId values do not participate in (de)serialization for now.
            if (node.get_type() == Type::ObjectId) {
                return {};
            }

            auto res = std::string();

            auto wrap = base.wrapper(node.get_type(), node.get_key());
            res += wrap.first;

            switch (node.get_type()) {
            case Type::Array:
            case Type::Map:
            case Type::Object: {
                const auto& children = node.get_children();
                for (auto it = children.cbegin(); it != children.cend(); ++it) {
                    res += render_node(**it, base);
                    if (it + 1 != children.cend()) {
                        res += base.element_gap(node.get_type());
                    }
                }
                break;
            }

            default: {
                res += base.leaf_value(node.get_value());
                break;
            }
            }

            res += wrap.second;
            return res;
        }

    } // namespace

    auto SerdeBase::to_string(const ObjectId& id) -> std::string {
        if (!id.available()) {
            throw std::invalid_argument("id is not available");
        }

        SerdeStruct ss(id);
        if (!ss.available()) {
            return {};
        }

        return render_node(ss.root(), *this);
    }

    auto SerdeBase::to_string(const std::string& key, const Value& v) -> std::string {
        return render_node(SerdeNode(key, v), *this);
    }

} // namespace gkit::core::reflect
