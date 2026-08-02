#include "gkit/core/reflect/serdebase.hpp"
#include "core/object_pool.hpp"
#include "gkit/core/reflect/registry.hpp"
#include <stdexcept>
#include <string>

namespace gkit::core::reflect {
    auto SerdeBase::seralize(const ObjectId& id) -> std::string {
        if (!id.available()) throw std::invalid_argument("id is not available");

        auto res = std::string();
        auto obj_ptr = ObjectPool::instance().deref_from(id);
        if (obj_ptr == nullptr) throw std::logic_error("obj is null");

        const auto* class_info = ClassDB::instance().find(obj_ptr->class_name());
        class_info->for_each_field([this, &class_info, obj_ptr, &res](const FieldDesc& field, const auto& getter, const auto*) {
            auto val_opt = class_info->get_field(obj_ptr, field.name);
            if (!val_opt.has_value()) return;
            res += this->seralize(field.name, val_opt.value());
        });

        return res;
    }
    
    auto SerdeBase::seralize(const std::string& key, const Value& v) -> std::string {
        auto res = std::string();
        auto ele_gap = std::string();
        auto value_wrap = WrapperPair();
        
        auto type = v.type();
        switch (type) {
            case Type::Array: {
                ele_gap = this->array_ele_gap();
                value_wrap = this->array_wrapper(key);
                break;
            }

            case Type::Map: {
                ele_gap = this->map_ele_gap();
                value_wrap = this->map_wrapper(key);
                break;
            }

            case Type::Object: {
                ele_gap = this->object_ele_gap();
                value_wrap = this->object_wrapper(key);
                break;
            }

            default:
                break;
        }

        res += value_wrap.first; // Value begin

        if (!key.empty()) {
            auto key_wrap = this->key_wrapper(key);
            res += key_wrap.first + key + key_wrap.second;
            res += this->kv_connect();
        }

        // elements to string
        switch (type) {
            case Type::Array: {
                const auto& arr = v.as_array();
                for (auto it = arr.cbegin(); it != arr.cend(); ++it) {
                    res += seralize("", *it);
                    if (it + 1 != arr.cend()) {
                        res += ele_gap;
                    }
                }
                break;
            }

            case Type::Map: {
                const auto& map = v.as_map();
                for (auto it = map.cbegin(); it != map.cend(); ++it) {
                    res += seralize(it->first, it->second);
                }
                break;
            }

            case Type::Object: {
                const auto* obj = v.as_object_ptr();
                if (obj == nullptr) {
                    break;
                }

                auto& db = ClassDB::instance();
                const auto* class_info = db.find(obj->class_name());

                class_info->for_each_field([this, &res, &class_info, obj](const FieldDesc& field, auto& getter, const auto*) {
                    auto val_opt = class_info->get_field(obj, field.name);
                    if (!val_opt.has_value()) return; // noreachable
                    res += this->seralize(field.name, val_opt.value());
                });

                break;
            }

            // Base data
            case Type::Bool: {
                res += std::to_string(v.as_bool());
                break;
            }

            case Type::Number: {
                if (v.is_number_float()) {
                    res += std::to_string(v.as_float());
                } else {
                    res += std::to_string(v.as_int64());
                }
                break;
            }

            case Type::String: {
                res += "\"" + v.as_string() + "\"";
                break;
            }

            default: {
                break;
            }
        }

        res += value_wrap.second; // Value end
        
        return res;
    }

} // namespace gkit::core::reflect