#pragma once

#include "gkit/core/object_id.hpp"
#include "gkit/core/unique_object.hpp"
#include "gkit/core/value.hpp"
#include <string>
#include <utility>

namespace gkit::core::reflect {
    class SerdeBase {
        std::string buffer {};
    public:
        SerdeBase()  = default;
        ~SerdeBase() = default;

        [[nodiscard]] auto seralize(const ObjectId& id) -> std::string;
        [[nodiscard]] auto seralize(const std::string& key, const Value& v) -> std::string;
        auto deseralize(std::string class_name) -> UniqueObject;
        
    protected:
        virtual auto kv_connect() -> std::string = 0;

        using WrapperPair = std::pair<std::string, std::string>;
        virtual auto key_wrapper(const std::string& name) -> WrapperPair = 0;
        virtual auto array_wrapper(const std::string& name) -> WrapperPair = 0;
        virtual auto map_wrapper(const std::string& name) -> WrapperPair = 0;
        virtual auto object_wrapper(const std::string& name) -> WrapperPair = 0;

        virtual auto array_ele_gap() -> std::string = 0;
        virtual auto map_ele_gap() -> std::string = 0;
        virtual auto object_ele_gap() -> std::string = 0;
    };
} // namespace gkit::core::reflect