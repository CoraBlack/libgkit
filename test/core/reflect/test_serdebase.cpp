#include "gkit/core/unique_object.hpp"
#include <gkit/core/reflect/registry.hpp>
#include <gkit/core/reflect/serdebase.hpp>
#include "gkit/core/value.hpp"
#include <iostream>

using gkit::core::Value;
using gkit::core::reflect::SerdeBase;

class SeralizeObject : public gkit::core::Object {
    int64_t num = 10;
    float f_num = 20.f;
    std::string str = "Hello my girl";
    gkit::core::Array arr = {
        Value(100),
        Value(200),
        Value(300)
    };

    gkit::core::Map map = {
        {"map_num", Value(100)},
        {"map_fnum", Value(200.f)},
        {"map_str", Value("")}
    };


public:
    static const char* c_name;
    static auto regist_method() -> void {
        auto& db = gkit::core::reflect::ClassDB::instance();
        db.regist<SeralizeObject>(c_name)
            .add_field(c_name, "num", &SeralizeObject::num)
            .add_field(c_name, "f_num", &SeralizeObject::f_num)
            .add_field(c_name, "str", &SeralizeObject::str)
            .add_field(c_name, "arr", &SeralizeObject::arr)
            .add_field(c_name, "map", &SeralizeObject::map);
    }
};

const char* SeralizeObject::c_name  = "SeralizeObject";

class Json : public SerdeBase {
    auto kv_connect() -> std::string override {
        return ":";
    }
    
    auto key_wrapper(const std::string& name) -> WrapperPair override {
        return {"\"", "\""};
    }

    auto array_wrapper(const std::string& name) -> WrapperPair override {
        return {"[", "]"};
    }

    auto map_wrapper(const std::string& name) -> WrapperPair override {
        return {"{", "}"};
    }

    auto object_wrapper(const std::string& name) -> WrapperPair override {
        return {"{", "}"};
    }

    auto array_ele_gap() -> std::string override {
        return ",";
    }

    auto map_ele_gap() -> std::string override {
        return ",";
    }

    auto object_ele_gap() -> std::string override {
        return ",";
    }
};

auto main() -> int {
    SeralizeObject::regist_method();
    auto json = Json();
    auto obj = gkit::core::UniqueObject::create<SeralizeObject>();
    auto serde_str = json.seralize(obj.get_id());
    std::cout << serde_str << '\n';
}