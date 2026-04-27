#include "gkit/core/reflection/class_db.hpp"

namespace gkit::core::reflection {

    FieldInfo::FieldInfo(std::string name, std::string type_name,
                         std::function<auto(const void*) -> std::optional<Value>> getter)
        : name_(std::move(name)), type_name_(std::move(type_name)), getter_(std::move(getter)) {}


    auto FieldInfo::name() const -> const std::string& {
        return name_;
    }


    auto FieldInfo::type_name() const -> const std::string& {
        return type_name_;
    }


    auto FieldInfo::get(const void* instance) const -> std::optional<Value> {
        return getter_ ? getter_(instance) : std::nullopt;
    }


    ClassInfo::ClassInfo(std::string class_name)
        : class_name_(std::move(class_name)) {}

    auto ClassInfo::class_name() const -> const std::string& {
        return class_name_;
    }


    auto ClassInfo::fields() const -> const std::vector<FieldInfo>& {
        return fields_;
    }


    auto ClassInfo::add_field(FieldInfo field) -> void {
        fields_.push_back(std::move(field));
    }


    auto ClassDB::get_class(std::string_view class_name) const -> const ClassInfo* {
        std::lock_guard<std::mutex> lock(class_map_mutex);
        auto it = class_map.find(std::string(class_name));
        if (it != class_map.end()) {
            return &(it->second);
        }
        return nullptr;
    }

} // namespace gkit::core::reflection
